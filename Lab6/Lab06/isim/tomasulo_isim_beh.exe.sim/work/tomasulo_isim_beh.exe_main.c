/**********************************************************************/
/*   ____  ____                                                       */
/*  /   /\/   /                                                       */
/* /___/  \  /                                                        */
/* \   \   \/                                                       */
/*  \   \        Copyright (c) 2003-2009 Xilinx, Inc.                */
/*  /   /          All Right Reserved.                                 */
/* /---/   /\                                                         */
/* \   \  /  \                                                      */
/*  \___\/\___\                                                    */
/***********************************************************************/

#include "xsi.h"

struct XSI_INFO xsi_info;



int main(int argc, char **argv)
{
    xsi_init_design(argc, argv);
    xsi_register_info(&xsi_info);

    xsi_register_min_prec_unit(-12);
    work_m_00000000000782618081_3457076406_init();
    work_m_00000000002628655610_1461632002_init();
    work_m_00000000000633878368_3385901664_init();
    work_m_00000000002024282335_1238703286_init();
    work_m_00000000001521767429_2848720641_init();
    work_m_00000000004134447467_2073120511_init();


    xsi_register_tops("work_m_00000000001521767429_2848720641");
    xsi_register_tops("work_m_00000000004134447467_2073120511");


    return xsi_run_simulation(argc, argv);

}
