/*Skript für Sleep Modes des MC

Stand: 06.11.2014
Version: 1.0.0
Letzte Änderung: TR

Historie:
-Ersterstellung
*/

#include "RTCInternalManagement.h"

static void _SwitchMck2FastRC( uint32_t dwMoscrcf, uint32_t dwPres );
static void RestoreWorkingClock( uint32_t PllarVal, uint32_t MckrVal );

/**
 * \brief Enter WAIT mode.
 * Enters wait mode based on
 *
 * \param waitPeriod 	Main On-Chip RC Oscillator Frequency Selection.
 * \param serialBaud    Processor Clock Prescaler.
 *
 */
void enterWaitMode(short waitPeriod, unsigned long serialBaud)
{
    uint32_t PllarVal =0;
    uint32_t MckrVal  =0;
    uint32_t MorVal  =0;
    uint8_t currentMinute =0;
    short waitMinutes = 0;

    //Set RTC to wake up after the defined amount of minutes
    RTC_GetTime(RTC, 0, &currentMinute, 0);

    waitMinutes = ((REG_RTC_TIMALR & RTC_TIMALR_MIN_Msk) >> RTC_TIMALR_MIN_Pos);

    while (waitMinutes <= currentMinute){
        waitMinutes += waitPeriod;
    }

    setALARM(waitMinutes);

    Serial.end();	
    delay(2); //wait for communication to stabilize

    //save current working clock settings
    PllarVal = REG_CKGR_PLLAR;
    MckrVal  = REG_PMC_MCKR;

    //Disable Brownout Detector
    REG_SUPC_MR |= SUPC_MR_KEY(0xA5) | SUPC_MR_BODDIS_DISABLE; 

    //Configure 4Mhz fast RC oscillator
    _SwitchMck2FastRC( CKGR_MOR_MOSCRCF_4_MHz, PMC_MCKR_PRES_CLK_1 );

    //Set wakeup input for fast startup
    pmc_set_fast_startup_input(PMC_FSMR_RTCAL);   //Enable Timer Wakeup

    //Enter Wait Mode
    pmc_enable_waitmode();

    //Restore working clock settings
    RestoreWorkingClock( PllarVal, MckrVal );

    //Enable Brownout Detector
    REG_SUPC_MR = (REG_SUPC_MR |SUPC_MR_KEY(0xA5)) & ~SUPC_MR_BODDIS; 

    //Reset Flags
    RTC_ClearSCCR(RTC, RTC_SCCR_ACKCLR | RTC_SCCR_ALRCLR | RTC_SCCR_SECCLR | RTC_SCCR_TIMCLR | RTC_SCCR_CALCLR );

    Serial.begin(serialBaud);
}

/**
 * \brief Switch MCK to FastRC (Main On-Chip RC Oscillator).
 *
 * \param moscrcf Main On-Chip RC Oscillator Frequency Selection.
 * \param pres    Processor Clock Prescaler.
 */
static void _SwitchMck2FastRC( uint32_t dwMoscrcf, uint32_t dwPres )
{
    /* Enable Fast RC oscillator but DO NOT switch to RC now . Keep MOSCSEL to 1 */
    PMC->CKGR_MOR = CKGR_MOR_MOSCSEL | (0x37 << 16) | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCRCEN ;
    /* Wait the Fast RC to stabilize */
    while ( !(PMC->PMC_SR & PMC_SR_MOSCRCS) ) ;

    /* Switch from Main Xtal osc to Fast RC */
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN ;
    /* Wait for Main Oscillator Selection Status bit MOSCSELS */
    while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) ) ;

    /* Disable Main XTAL Oscillator */
    PMC->CKGR_MOR = (0x37 << 16) | CKGR_MOR_MOSCRCEN ;

    /* Change frequency of Fast RC oscillator */
    PMC->CKGR_MOR = (0x37 << 16) | PMC->CKGR_MOR | dwMoscrcf ;
    /* Wait the Fast RC to stabilize */
    while ( !(PMC->PMC_SR & PMC_SR_MOSCRCS) ) ;

    /* Switch to main clock */
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (uint32_t)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK ;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) ;
    PMC->PMC_MCKR = (PMC->PMC_MCKR & (uint32_t)~PMC_MCKR_PRES_Msk) | dwPres ;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) ;

    /* Stop PLL A */
    /* STMODE must be set at 2 when the PLLA is Off */
    PMC->CKGR_PLLAR = 0x2 << 14 ;
}

/**
 * \brief Restore working clock.
 * Here working clock must be running from PLL
 * and external crystal oscillator is used.
 */
static void RestoreWorkingClock( uint32_t PllarVal, uint32_t MckrVal )
{
    /* switch to slow clock first */
    pmc_switch_mck_to_sclk(PMC_MCKR_PRES_CLK_1);

    /* Restart Main Oscillator */
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN ;
    while ( !(PMC->PMC_SR & PMC_SR_MOSCXTS) ) ;
    /* Switch to moscsel */
    PMC->CKGR_MOR = (0x37 << 16) | (0x3F<<8) | CKGR_MOR_MOSCRCEN | CKGR_MOR_MOSCXTEN | CKGR_MOR_MOSCSEL ;
    while ( !(PMC->PMC_SR & PMC_SR_MOSCSELS) ) ;

    /* Switch to main oscillator */
    pmc_switch_mck_to_mainck(PMC_MCKR_PRES_CLK_1);

    /* Restart PLL A */
    pmc_enable_pllack((PllarVal&CKGR_PLLAR_MULA_Msk)>>CKGR_PLLAR_MULA_Pos,
            (PllarVal&CKGR_PLLAR_PLLACOUNT_Msk)>>CKGR_PLLAR_PLLACOUNT_Pos,
            (PllarVal&CKGR_PLLAR_DIVA_Msk)>>CKGR_PLLAR_DIVA_Pos
            );

    /* Switch to fast clock */
    PMC->PMC_MCKR = (MckrVal & (uint32_t)~PMC_MCKR_CSS_Msk) | PMC_MCKR_CSS_MAIN_CLK ;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) ;

    PMC->PMC_MCKR = MckrVal ;
    while ( !(PMC->PMC_SR & PMC_SR_MCKRDY) ) ;
}

