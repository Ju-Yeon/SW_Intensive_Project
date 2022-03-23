#include "my_lib.h"

/* Initialize LED (RED,BLUE) */
void init_LED(void)
{
    /* Reset PC1 & PC2 in IOCR0*/
    PORT10_IOCR0 &= ~((0x1F) << PC1);
    PORT10_IOCR0 &= ~((0x1F) << PC2);

    /* Set PC1 & PC2 with push-pull(2b10000) */
    PORT10_IOCR0 |= ((0x10) << PC1);
    PORT10_IOCR0 |= ((0x10) << PC2);
}

/* Initialize Switch2 */
void init_Switch(void)
{
    /* Reset PC0 & PC1 in IOCR0*/
    PORT02_IOCR0 &= ~((0x1F) << PC1);
    PORT02_IOCR0 &= ~((0x1F) << PC0);

    /* Set PC0 & PC1 with pull-up(2b0xx10) */
    PORT02_IOCR0 |= ((0x2) << PC1);
    PORT02_IOCR0 |= ((0x2) << PC0);
}

void init_GTM_TOM0_PWM_RGB(void)
{
    /* GTM Enable */
    // Password Access to unlock WDTCPU0CON0
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    // Modify Access to clear ENDINIT bit
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) & ~(1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);

    GTM_CLC &= ~(1 << DISR);                        // Enable GTM Module

    // Password Access to unlock WDTCPU0CON0
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    // Modify Access to set ENDINIT bit
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);

    while((GTM_CLC & (1 << DISS)) != 0);            // Wait until module is enabled

    /* GTM Clock Setting */
    GTM_CMU_FXCLK_CTRL &= ~((0xF) << FXCLK_SEL);    // Input clock of CMU_FXCLK : CMU_GCLK_EN

    GTM_CMU_CLK_EN &= ~((0x3) << EN_FXCLK);         // Enable all CMU_FXCLK
    GTM_CMU_CLK_EN |= ((0x2) << EN_FXCLK);

    /* GTM TOM0 PWM Setting channel 0 ~ channel 7*/
    // channel 2
    GTM_TOM0_TGC0_GLB_CTRL &= ~((0x3) << UPEN_CTRL2);   // TOM0 channel 2 enable update of
    GTM_TOM0_TGC0_GLB_CTRL |= ((0x2) << UPEN_CTRL2);    // register CM0, CM1, CLK_SRC
    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << FUPD_CTRL2);  // Enable force update of TOM0 channel 1
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << FUPD_CTRL2);
    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << RSTCN0_CH1);  // Reset CN0 of TOM0 channel 1 on force update
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << RSTCN0_CH1);
    GTM_TOM0_TGC0_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL2); // Enable channel 1 on an update trigger
    GTM_TOM0_TGC0_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL2);
    GTM_TOM0_TGC0_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL2); // Enable channel 1 output on an update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL2);

    GTM_TOM0_CH2_CTRL |= (1 << SL);                     // High signal level for duty cycle
    GTM_TOM0_CH2_CTRL &= ~((0x7) << CLK_SRC_SR);        // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH2_CTRL |= (1 << CLK_SRC_SR);
    GTM_TOM0_CH2_SR0 = 12500 - 1;                       // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH2_SR1 = 0;                               // Duty cycle = 0
    GTM_TOUTSEL6 &= ~((0x3) << SEL11);                  // TOUT107 : TOM0 channel 2

    // channel 3
    GTM_TOM0_TGC0_GLB_CTRL &= ~((0x3) << UPEN_CTRL3);   // TOM0 channel 3 enable update of
    GTM_TOM0_TGC0_GLB_CTRL |= ((0x2) << UPEN_CTRL3);    // register CM0, CM1, CLK_SRC
    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << FUPD_CTRL3);  // Enable force update of TOM0 channel 1
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << FUPD_CTRL3);
    GTM_TOM0_TGC0_FUPD_CTRL &= ~((0x3) << RSTCN0_CH3);  // Reset CN0 of TOM0 channel 1 on force update
    GTM_TOM0_TGC0_FUPD_CTRL |= ((0x2) << RSTCN0_CH3);
    GTM_TOM0_TGC0_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL3); // Enable channel 1 on an update trigger
    GTM_TOM0_TGC0_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL3);
    GTM_TOM0_TGC0_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL3); // Enable channel 1 output on an update trigger
    GTM_TOM0_TGC0_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL3);

    GTM_TOM0_CH3_CTRL |= (1 << SL);                     // High signal level for duty cycle
    GTM_TOM0_CH3_CTRL &= ~((0x7) << CLK_SRC_SR);        // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH3_CTRL |= (1 << CLK_SRC_SR);
    GTM_TOM0_CH3_SR0 = 12500 - 1;                       // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH3_SR1 = 0;                               // Duty cycle = 0
    GTM_TOUTSEL6 &= ~((0x3) << SEL9);                   // TOUT105 : TOM0 channel 3

    // channel 15
    GTM_TOM0_TGC1_GLB_CTRL &= ~((0x3) << UPEN_CTRL15);  // TOM0 channel 15 enable update of
    GTM_TOM0_TGC1_GLB_CTRL |= ((0x2) << UPEN_CTRL15);   // register CM0, CM1, CLK_SRC
    GTM_TOM0_TGC1_FUPD_CTRL &= ~((0x3) << FUPD_CTRL15); // Enable force update of TOM0 channel 15
    GTM_TOM0_TGC1_FUPD_CTRL |= ((0x2) << FUPD_CTRL15);
    GTM_TOM0_TGC1_FUPD_CTRL &= ~((0x3) << RSTCN0_CH15); // Reset CN0 of TOM0 channel 15 on force update
    GTM_TOM0_TGC1_FUPD_CTRL |= ((0x2) << RSTCN0_CH15);
    GTM_TOM0_TGC1_ENDIS_CTRL &= ~((0x3) << ENDIS_CTRL15);// Enable channel 15 on an update trigger
    GTM_TOM0_TGC1_ENDIS_CTRL |= ((0x2) << ENDIS_CTRL15);
    GTM_TOM0_TGC1_OUTEN_CTRL &= ~((0x3) << OUTEN_CTRL15);// Enable channel 15 output on an update trigger
    GTM_TOM0_TGC1_OUTEN_CTRL |= ((0x2) << OUTEN_CTRL15);

    GTM_TOM0_CH15_CTRL |= (1 << SL);                     // High signal level for duty cycle
    GTM_TOM0_CH15_CTRL &= ~((0x7) << CLK_SRC_SR);        // Clock source : CMU_FXCLK(1) = 6250 kHz
    GTM_TOM0_CH15_CTRL |= (1 << CLK_SRC_SR);
    GTM_TOM0_CH15_SR0 = 12500 - 1;                       // PWM freq. = 6250 kHz / 12500 = 500 Hz
    GTM_TOM0_CH15_SR1 = 0;                               // Duty cycle = 0
    GTM_TOUTSEL0 &= ~((0x3) << SEL7);                    // TOUT7 : TOM0 channel 3

    GTM_TOM0_TGC0_GLB_CTRL |= (1 << HOST_TRIG);         // Trigger request signal to update
    GTM_TOM0_TGC1_GLB_CTRL |= (1 << HOST_TRIG);         // Trigger request signal to update
}


void init_CCU6(myCCU6 *CCU6, int ms, int int_priority)
{
    /* CCU60 T12 Setting */

    /* Password Access to unlock WDTSCON0 */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    /* Modify Access to clear ENDINIT bit */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) & ~ (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);

    CCU6->CLC &= ~(1 << DISR);                     // Enable CCU61 Module

    /* Password Access to unlock WDTSCON0 */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) & ~(1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) != 0);

    /* Modify Access to clear ENDINIT bit */
    SCU_WDT_CPU0CON0 = ((SCU_WDT_CPU0CON0 ^ 0xFC) | (1 << LCK)) | (1 << ENDINIT);
    while((SCU_WDT_CPU0CON0 & (1 << LCK)) == 0);

    while((CCU6->CLC & (1 << DISS)) != 0);       // Wait until module is enabled

    CCU6->TCTR0 &= ~((0x7) << T12CLK);           // f_T12 = f_CCU6 / prescaler, 100MHz
    CCU6->TCTR0 |= ((0x3) << T12CLK);            // F_CCU6 = 100 MHz, prescaler = 2048
    CCU6->TCTR0 |= (1 << T12PRE);                // f_T12 = 48828 Hz

    CCU6->TCTR0 &= ~(1 << CTM);                  // T12 always counts up and continues counting
                                                 // from zero after reaching the period value

    CCU6->T12PR = (48828*ms)/1000 - 1;
    CCU6->TCTR4 |= (1 << T12STR);                // Interrupt freq. = 2 Hz

    CCU6->T12 = 0;                               // Clear T12 counting value

    /* CCU6 T12 Interrupt Setting */
    CCU6->INP &= ~((0x3) << INPT12);             // Service Request output SR0 is selected

    CCU6->IEN |= (1 << ENT12PM);                 // Enable Interrupt for T12 Period-Match

    /* SRC Interrupt Setting For CCU6 */
    if( (int)CCU6 == CCU60_BASE )
    {
        SRC_CCU60_SR0 &= ~((0xFF) << SRPN);           // Set Priority
        SRC_CCU60_SR0 |= (int_priority << SRPN);
        SRC_CCU60_SR0 &= ~((0x3) << TOS);             // CPU0 services
        SRC_CCU60_SR0 |= (1 << SRE);                  // Service Request is enabled
    }
    else
    {
        SRC_CCU61_SR0 &= ~((0xFF) << SRPN);           // Set Priority
        SRC_CCU61_SR0 |= (int_priority << SRPN);
        SRC_CCU61_SR0 &= ~((0x3) << TOS);             // CPU0 services
        SRC_CCU61_SR0 |= (1 << SRE);                  // Service Request is enabled
    }

    /* CCU6 T12 Start */
    CCU6->TCTR4 = (1 << T12RS);                  // T12 starts counting
}
