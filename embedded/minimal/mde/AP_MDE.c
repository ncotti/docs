/**
*	\file       AP_MDE.c
*	\brief      Maquina/s de Estado
*	\details    Descripcion detallada del archivo
*	\author     cotti
*	\date       24-07-2023 09:36:14
*/
/************************************************************************************************************
 *** INCLUDES
 ************************************************************************************************************/
#include "AP_MDE.h"
#include "AP_FuncionesMDE.h"

/************************************************************************************************************
 *** DEFINES PRIVADOS AL MODULO
 ************************************************************************************************************/
#define		RESET		0	
//Maquina: MdE1
#define		ESTADO1		1	// < Maquina: MdE1 >
#define		ESTADO2		2	// < Maquina: MdE1 >

/************************************************************************************************************
 *** MACROS PRIVADAS AL MODULO
 ************************************************************************************************************/
#define     m_init()

#define     m_respuesta_1()

#define     m_respuesta_2()


/************************************************************************************************************
 *** PROTOTIPO DE FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/
static int MdE1 ( int );

/************************************************************************************************************
 *** VARIABLES GLOBALES PUBLICAS
 ************************************************************************************************************/

/************************************************************************************************************
 *** FUNCIONES PRIVADAS AL MODULO
 ************************************************************************************************************/

/**
*	\fn      static int MdE1 ( int Estado )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  cotti
*	\date    24-07-2023 09:36:14
*   \param   [in] Estado: caso dentro de la maquina (estado actual)
*	\return  int : estado siguiente
*/

static int MdE1 ( int  Estado )
{
    switch ( Estado )
    {

        case RESET :
            m_init();

            Estado = ESTADO1;

            break;

        case ESTADO1 :
            if ( s_evento1() )
            {
                m_respuesta_1();

                Estado = ESTADO2;
            }

            break;

        case ESTADO2 :
            if ( s_evento_2() )
            {
                m_respuesta_2();

                Estado = ESTADO1;
            }

            break;

        default:
            Estado = RESET ;
            break;

    }
    return Estado ;
}

/************************************************************************************************************
 *** FUNCIONES GLOBALES AL MODULO
 ************************************************************************************************************/

/**
*	\fn      void MaquinaDeEstados ( void )
*	\brief   Coloque aqui su descripcion
*	\details Amplie la descripcion
*	\author  cotti
*	\date    24-07-2023 09:36:14
*   \param   void
*	\return  void
*/
void MaquinaDeEstados ( void )
{
    static int estados_MdE1 = RESET;

    // Coloque su codigo aqui

    estados_MdE1 = MdE1( estados_MdE1 );


    return ;
}

