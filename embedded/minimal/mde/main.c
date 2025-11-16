/**
*	\file       main.c
*	\brief      Proyecto: test_umodel
*	\details    Descripcion detallada del archivo
*	\author     cotti
*	\date       24-07-2023 09:36:14
*/

/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/

#include "AP_MDE.h"
#include "AP_Inicializacion.h"

void main ( void )
{
    Inicializar( );

    while ( 1 )
    {
        MaquinaDeEstados();
    }
    return ;
}

