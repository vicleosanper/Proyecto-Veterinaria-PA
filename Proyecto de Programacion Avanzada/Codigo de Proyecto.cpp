#include <windows.h>
#include <time.h>
#include <stdlib.h> //Libreria que incluye la funcion atoi y atof
#include "resource.h"
#include <string>
#include <fstream>//LIBRERIA PARA Guardar y leer datos
#include <string>
#include <cctype>

using namespace std;
#pragma warning(disable:4996)

//fstream miArchivo;


#define TM_RELOJ 3000

bool fotoagregada = false;

time_t tiempo = time(NULL);//variables donde guardo el valor de la funcion time.
struct tm *tlocal; //estructura donde obtengo el tiempo

char FECHA_ACTUAL[15]; //ARREGLO PARA TENER LA FECHA DE MANERA GLOBAL
char HORA_ACTUAL[15];


HWND hVentanaInfodelDoctor;//Solo muestrainfo del doctor
HWND hVentanaInfodelDoctorEDIT;
HWND hVentanaInicioSession;
HWND hVentanaAgenda;
HWND hVentanaNuevaCita;
HWND hVentanaPago;
HWND hVentanaFinal;
HWND hVentanaAgendaMostrar; //Ventana que mostrara la informacion
HWND hVentanaAgendaModificar;
HWND hVentanaAgendaEliminar;

//Funciones para validar
void Ordenar_estructuras();


int INICIOSESION(char[],char[]);
int validar_numeroTEL(char[]);
int validar_nombre(char[]);
int validar_costo(char[]);
int validar_fecha(string, string);
int validar_fecha2(string, string);
int validar_fecha_y_hora(string, string, string, string);


int validar_fecha_y_hora_CON_ID(string FECHA_ACTUAL, string HORA_ACTUAL, string FECHA_INGRESADA, string HORA_INGRESADA, int ID);
int validar_fecha2_CON_ID(string, string,int);

int validar_hora(string, string);
int validar_hora_FORMATO_12(string, string);
int ValidacionLongitudDOC(HWND);
int ValidacionLongitud(HWND);



BOOL CALLBACK FuncionVentanaInfoDoctor(HWND, UINT, WPARAM, LPARAM); //Solo muestra
BOOL CALLBACK FuncionVentanaInfoDoctorEDIT(HWND, UINT, WPARAM, LPARAM); //Edita la informacion
BOOL CALLBACK FuncionIniciodeSession(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaAgenda(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaNuevaCita(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaPago(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaFINAL(HWND, UINT, WPARAM, LPARAM);


BOOL CALLBACK FuncionVentanaAgendaModificar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaAgendaMostrar(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK FuncionVentanaAgendaEliminar(HWND, UINT, WPARAM, LPARAM);


struct Agenda {

	char Fecha[20];
	char Hora[15];
	char Nombre[35];
	char NombrePersona[35];
	char Telefono[12];
	char Especie[11];
	char NombreMascota[30];
	char Motivo[100];
	char Costo[8];
	int ID;
	
	char ch_DirFoto[MAX_PATH] = "";
	char ch_DirFoto2[MAX_PATH]= "";

	Agenda* next;
	Agenda* prev;


}*origen, *aux;

struct aux_para_ordenar {

	char Fecha[20];
	char Hora[15];
	char Nombre[35];
	char NombrePersona[35];
	char Telefono[12];
	char Especie[11];
	char NombreMascota[30];
	char Motivo[100];
	char Costo[8];
	int ID;

	char ch_DirFoto[MAX_PATH] = "";
	char ch_DirFoto2[MAX_PATH] = "";
	


}*aux_para_ordenar_lista;

struct DOCTOR {

	char nombreDoc[50];
	char cedula[50];
	char DirFileDoc[MAX_PATH]= " ";

}*doc;


//FUNCIONES DE GUARDADO Y LECTURA
void cargar();
void guardar(Agenda*);
void cargarDOC();
void guardarDOC(DOCTOR*);

char LogContraseña[30];   //Variables donde vamos a guardar lo del documento del texto
char LogUsuario[30];

char Usuario[30];
char Contraseña[30];

//IMAGEN DEL DOCTOR
HBITMAP hBitmap; 
HWND himagen;

//Instancia global para poder mostrar nombre
HINSTANCE hGlobalInstance; 


int globalBuscadorID;
int globalID = 0;
bool Habilitador = false;
bool primeracita = false;

char chDireccionDOC[MAX_PATH] = " "; //VARIABLES PARA VALIDAR
char chDirFile[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos
char chDirFile2[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos

bool FILTRO_APLICADO = false;
bool SI_HAY_CITAS = false;
string FECHA_FILTRO;


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdShow) {

	cargar();
	
	cargarDOC();



	aux = origen;

	hVentanaInfodelDoctor = CreateDialog(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, FuncionVentanaInfoDoctor); //Solo muestrainfo del doctor
	hVentanaInfodelDoctorEDIT = CreateDialog(hInst, MAKEINTRESOURCE(IDD_Editdoc), NULL, FuncionVentanaInfoDoctorEDIT);
	hVentanaInicioSession = CreateDialog(hInst, MAKEINTRESOURCE(IDD_Incio), NULL, FuncionIniciodeSession);
	hVentanaAgenda = CreateDialog(hInst, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
	//hVentanaNuevaCita = CreateDialog(hInst, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita); //Creamos un nuevo Dialogo
	hVentanaFinal = CreateDialog(hInst, MAKEINTRESOURCE(IDD_VENTANAFINAL), NULL, FuncionVentanaFINAL);


	//hVentanaPago = CreateDialog(hInst, MAKEINTRESOURCE(IDD_PAGO), NULL, FuncionVentanaPago);
	//hVentanaAgendaEliminar = CreateDialog(hInst, MAKEINTRESOURCE(IDD_ELIMINARCITA), NULL, FuncionVentanaAgendaEliminar); //
	//hVentanaAgendaModificar = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MODIFICAR), NULL, FuncionVentanaAgendaModificar);//
	//hVentanaAgendaMostrar = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MOSTRARinfo), NULL, FuncionVentanaAgendaMostrar);//

	

	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));
	ShowWindow(hVentanaInicioSession, SW_SHOW);
	

	while (GetMessage(&msg, 0, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return 0;

}

#pragma region VENTANAS_


		BOOL CALLBACK FuncionVentanaInfoDoctorEDIT(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {

				case WM_INITDIALOG: {
					aux = origen; 
					
				}
								break;

			case WM_COMMAND: {

				if (LOWORD(wParam) == IDC_AgregarFoto && HIWORD(wParam) == BN_CLICKED) { //AGREGAR FOTO

					
					OPENFILENAME ofn; //ESTRUCTURA 
					ZeroMemory(&ofn, sizeof(ofn));
					

					ofn.hwndOwner = hWnd;
					ofn.lStructSize = sizeof(ofn);
					ofn.lpstrFile = chDireccionDOC;
					ofn.nMaxFile = MAX_PATH;
					ofn.lpstrDefExt = "txt";
					ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
					ofn.lpstrFilter = "Text Files\0*.txt\0All Files\0*.*\0";

					if (GetOpenFileName(&ofn)) { //Nos regresara un valor verdadero al elegir una imagen y dar en aceptar

						himagen = GetDlgItem(hWnd, IDC_imagen);
						hBitmap = (HBITMAP)LoadImage(NULL, chDireccionDOC, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
						SendMessage(himagen, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
						fotoagregada = true;

					}

				}

				else if (LOWORD(wParam) == IDC_CANCELAR && HIWORD(wParam) == BN_CLICKED) {

					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

				}
				else if (LOWORD(wParam) == IDC_ACEPTAR && HIWORD(wParam) == BN_CLICKED) {
			
					bool identificador;

					identificador = ValidacionLongitudDOC(hWnd); //Validamos el que no haya espacios vacios

					if (identificador == false) { //Si no esta todo relleno
						break;
					}

					char cValidaciondeDATOS[50];
					int VALIDADO = 0;

					HWND hEditBox = GetDlgItem(hWnd, IDC_EDIT1); //Creamos un identificador para el edit control
					int longitud = GetWindowTextLength(hEditBox); //Le damos a "longitud" la cantidad de caracteres ingresados
					GetWindowText(hEditBox, cValidaciondeDATOS, longitud + 1);

					if (identificador == false) {
						MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
						break;
					}
					else if (validar_nombre(cValidaciondeDATOS) == 0) {

						MessageBox(hWnd, "PorFavor ingrese un nombre valido", "ERROR", MB_OK);
						break;
					}
					else {

						VALIDADO = VALIDADO + 1;
					}

							 HWND hEditBox2 = GetDlgItem(hWnd, IDC_EDIT2);
							int longitud2 = GetWindowTextLength(hEditBox2);
							GetWindowText(hEditBox2, cValidaciondeDATOS, longitud2 + 1);

							if (identificador == false) {
								MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
								break;
							}
							else if (validar_nombre(cValidaciondeDATOS) == 0) { //Si el usuario ingresa letras

								MessageBox(hWnd, "PorFavor ingrese una Cedula Valida", "ERROR", MB_OK);
								break;
							}
							else {

								VALIDADO = VALIDADO + 1;
							}

							//Verificamos si se agrego foto
							if (fotoagregada == true) {

								VALIDADO = VALIDADO + 1;

							}
							else { //SI NO AGREGO FOTO

								MessageBox(hWnd, "PorFavor ingrese una fotografia", "ERROR", MB_OK);
								break;
							}

								if (VALIDADO == 3) {

									if (doc == NULL) { //SI NO TENEMOS NADA AUN EN LA ESTRUCTURA, LA CREAMOS
										doc = new DOCTOR;
									}
									

									GetWindowText(hEditBox2, doc->cedula, longitud2 + 1);
									GetWindowText(hEditBox, doc->nombreDoc, longitud + 1);
									strcpy(doc->DirFileDoc, chDireccionDOC); //MANDAMOS LA DIRECCION DE LA IMAGEN

								}

			


					//DestroyWindow(hWnd);
					hVentanaInfodelDoctor = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, FuncionVentanaInfoDoctor); //Le mandamos la instancia global
					ShowWindow(hWnd, SW_HIDE);
					DestroyWindow(hVentanaAgenda);
					DestroyWindow(hVentanaNuevaCita);
					hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
					hVentanaNuevaCita = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita); 
					ShowWindow(hVentanaInfodelDoctor, SW_SHOW);
				}

				else if (LOWORD(wParam) == BTN_INFODOC && HIWORD(wParam) == BN_CLICKED) { //Info del doctor
					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

				}
				else if ((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)) { //SALIR

					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaFinal, SW_SHOW);

				}
				else if ((LOWORD(wParam) == BTN_NUEVACITA && HIWORD(wParam) == BN_CLICKED)) { //Nueva Cita


					ShowWindow(hWnd, SW_HIDE);
					hVentanaNuevaCita = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita); //Creamos un nuevo Dialogo
					ShowWindow(hVentanaNuevaCita, SW_SHOW);

				}

				else if ((LOWORD(wParam) == BTN_AGENDA && HIWORD(wParam) == BN_CLICKED)) {

				ShowWindow(hWnd, SW_HIDE);
				ShowWindow(hVentanaAgenda, SW_SHOW);

			    }
			}
							 break;

				case WM_CLOSE:
					break;

				case WM_DESTROY:
					//PostQuitMessage(120);
					break;

				default:
					break;
			}
			return FALSE;


		}

		BOOL CALLBACK FuncionIniciodeSession(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {

				/*
				case WM_CREATE:

				break;
				*/

				case WM_INITDIALOG: {
					aux = origen; //Para que empiece a recorrer desde el principio

					HBITMAP foto1 = (HBITMAP)LoadImage(NULL,"C:\\Users\\leona\\Documents\\LMAD\\3er Semestre\\PROGRAMACIÓN\\Proyecto de Programacion Avanzada\\Imagenes BITMAP\\Logo.bmp", IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_LOGIN);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);

				}
								break;

				case WM_COMMAND: {

					if (LOWORD(wParam) == IDC_BUTTON2 && HIWORD(wParam) == BN_CLICKED) {
						DestroyWindow(hWnd);

					}
					else if (LOWORD(wParam) == IDC_BUTTON1 && HIWORD(wParam) == BN_CLICKED) {

						int longitud;

						HWND hUsuario = GetDlgItem(hWnd, IDC_EDIT1);
						longitud = GetWindowTextLength(hUsuario); //Sacamos la longitud
						GetWindowText(hUsuario, LogUsuario, longitud + 1); //Le mandamos lo ingresado en la casilla del doctor

						HWND hContraseña = GetDlgItem(hWnd, IDC_EDIT2);
						longitud = GetWindowTextLength(hContraseña); //Sacamos la longitud
						GetWindowText(hContraseña, LogContraseña, longitud + 1); //Le mandamos lo ingresado en la casilla de la cedula

						if (INICIOSESION(LogUsuario,LogContraseña) == 1) {

							ShowWindow(hWnd, SW_HIDE);
							ShowWindow(hVentanaAgenda, SW_SHOW); //Enseñamos la ventana

						}
						else if (INICIOSESION(LogUsuario, LogContraseña) == 0) {

							MessageBox(hWnd, "ERROR", "El nombre o la contraseña es incorrecto", MB_OK);

						}
				
					}
				}
					break;

				case WM_CLOSE:
					break;

				case WM_DESTROY:
					PostQuitMessage(120);
					break;

				default:
					break;
			}
			return FALSE;
		}

		BOOL CALLBACK FuncionVentanaAgenda(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {
				case WM_INITDIALOG:
			   {
					       Ordenar_estructuras();

						   if (doc != NULL) {
							   HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 70, 70, LR_LOADFROMFILE);
							   HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen);
							   SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
							   SetDlgItemText(hWnd, IDC_Nombredeldoctor, doc->nombreDoc);

							   if (strlen(doc->nombreDoc) > 0) {

								   SetDlgItemText(hWnd, IDC_DR, "Dr.");

							   }
							   else {

								   SetDlgItemText(hWnd, IDC_DR, "");

							   }
						   }
						   else {

							   SetDlgItemText(hWnd, IDC_DR, "");

						   }

							//Obtenemos los identificadores de las ventanas
							HWND hbtnPagar = GetDlgItem(hWnd, IDCagenda_PAGAR);
							HWND hbtnDesplegar = GetDlgItem(hWnd, IDCagenda_DESPLEGAR);
							HWND hbtnModificar = GetDlgItem(hWnd, IDC_Modificar);
							HWND hbtnEliminar = GetDlgItem(hWnd, IDC_ELIMINAR);
							HWND hListaAgenda = GetDlgItem(hWnd, IDC_CITAS); //En la list box

							EnableWindow(hbtnPagar, false);
							EnableWindow(hbtnDesplegar, false);    //Inahilitamos los botones
							EnableWindow(hbtnModificar, false);
							EnableWindow(hbtnEliminar, false);

					         
							aux = origen;

							if (FILTRO_APLICADO == false) { //Vemos si el usuario ingreso una fecha en el filtro

								//Ciclo para encontrar nodo sin informacion
								if (origen != NULL) {

									SI_HAY_CITAS = true;

									while (aux != NULL) {

										SendMessage(hListaAgenda, LB_ADDSTRING, 0, (LPARAM)aux->NombreMascota);

										//Obtenemos el numero de elementos en el cuadro de la lista
										int num_elementosdelalista = SendMessage(hListaAgenda, LB_GETCOUNT, NULL, NULL);

										// LB_SETITEMDATA Establece un valor asociado con el elemento especificado en un cuadro de lista
										SendMessage(hListaAgenda, LB_SETITEMDATA, (WPARAM) --num_elementosdelalista, (LPARAM)aux->ID); //Le enviamos el valor al ID

										aux = aux->next;

									}

								}
								else {

									SI_HAY_CITAS = false;

								}

							}
							else {

								//FECHA INGRESADA
								string str_dia, str_mes, str_año;
								int dia, mes, año;


								aux = origen;

								if (origen != NULL) {

									while (aux != NULL) {

										if (validar_fecha(aux->Fecha, FECHA_FILTRO) == 2) {//Si son exactamente la misma fecha || La variable FECHA es GLOBAL

											SendMessage(hListaAgenda, LB_ADDSTRING, 0, (LPARAM)aux->NombreMascota);

											//Obtenemos el numero de elementos en el cuadro de la lista
											int num_elementosdelalista = SendMessage(hListaAgenda, LB_GETCOUNT, NULL, NULL);

											// LB_SETITEMDATA Establece un valor asociado con el elemento especificado en un cuadro de lista
											SendMessage(hListaAgenda, LB_SETITEMDATA, (WPARAM) --num_elementosdelalista, (LPARAM)aux->ID); //Le enviamos el valor al ID


										}


										aux = aux->next;

									}


								}
								

							}


							
			        


			   }
							break;

				case WM_COMMAND:

				if (LOWORD(wParam) == IDC_CITAS && HIWORD(wParam) == LBN_SELCHANGE) { // ListBox
			
					HWND hbtnPagar = GetDlgItem(hWnd, IDCagenda_PAGAR);
					HWND hbtnDesplegar = GetDlgItem(hWnd, IDCagenda_DESPLEGAR);  //Creamos un identificador para cada boton
					HWND hbtnModificar = GetDlgItem(hWnd, IDC_Modificar);
					HWND hbtnEliminar = GetDlgItem(hWnd, IDC_ELIMINAR);
					EnableWindow(hbtnPagar, true);
					EnableWindow(hbtnDesplegar, true);
					EnableWindow(hbtnModificar, true); //Habilitamos los botones
					EnableWindow(hbtnEliminar, true);
			
				}

				else if (LOWORD(wParam) == IDCagenda_PAGAR && HIWORD(wParam) == BN_CLICKED) { // btn_Pagar
			
					HWND hListaAgenda = GetDlgItem(hWnd, IDC_CITAS);
					int selIndex = (int)SendMessage(hListaAgenda, LB_GETCURSEL, NULL, NULL);//Posicionando en el lugar a donde clicaste 
					globalBuscadorID = SendMessage(hListaAgenda, LB_GETITEMDATA, (WPARAM)selIndex, NULL);//Valor implicito
			

					DestroyWindow(hWnd);
					hVentanaPago = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_PAGO), NULL, FuncionVentanaPago);
					ShowWindow(hVentanaPago, SW_SHOW);
				}

				else if (LOWORD(wParam) == IDCagenda_DESPLEGAR && HIWORD(wParam) == BN_CLICKED) { // btn_Desplegar
			
					HWND hListaAgenda = GetDlgItem(hWnd, IDC_CITAS);
					int selIndex = (int)SendMessage(hListaAgenda, LB_GETCURSEL, NULL, NULL);//Posicionando en el lugar a donde clicaste 
					globalBuscadorID = SendMessage(hListaAgenda, LB_GETITEMDATA, (WPARAM)selIndex, NULL);//Valor implicito || Obtiene el valor definido por la aplicación asociado con el elemento del cuadro de lista especificado
			

					DestroyWindow(hWnd);
					hVentanaAgendaMostrar = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_MOSTRARinfo), NULL, FuncionVentanaAgendaMostrar);
					ShowWindow(hVentanaAgendaMostrar, SW_SHOW);
				}

				else if (LOWORD(wParam) == IDC_Modificar && HIWORD(wParam) == BN_CLICKED) { //MODIFICAR
					#pragma region SELECCION_DE_LISTA
					HWND hListaAgenda = GetDlgItem(hWnd, IDC_CITAS);
					int selIndex = (int)SendMessage(hListaAgenda, LB_GETCURSEL, NULL, NULL);//Posicionando en el lugar a donde clicaste 
					globalBuscadorID = SendMessage(hListaAgenda, LB_GETITEMDATA, (WPARAM)selIndex, NULL);//Valor implicito || Obtiene el valor definido por la aplicación asociado con el elemento del cuadro de lista especificado
			

					DestroyWindow(hWnd);
					hVentanaAgendaModificar = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_MODIFICAR), NULL, FuncionVentanaAgendaModificar); //Creamos el dialogo
					ShowWindow(hVentanaAgendaModificar, SW_SHOW);
				}

				else if (LOWORD(wParam) == IDC_ELIMINAR && HIWORD(wParam) == BN_CLICKED) { //ELIMINAR
			
					HWND hListaAgenda = GetDlgItem(hWnd, IDC_CITAS);
					int selIndex = (int)SendMessage(hListaAgenda, LB_GETCURSEL, NULL, NULL); //Posicionando en el lugar a donde clicaste 
					globalBuscadorID = SendMessage(hListaAgenda, LB_GETITEMDATA, (WPARAM)selIndex, NULL); //Valor implicito || Obtiene el valor definido por la aplicación asociado con el elemento del cuadro de lista especificado
			

					DestroyWindow(hWnd);
					hVentanaAgendaEliminar = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_ELIMINARCITA), NULL, FuncionVentanaAgendaEliminar);
					ShowWindow(hVentanaAgendaEliminar, SW_SHOW);
				}

				else if (LOWORD(wParam) == BTN_INFODOC && HIWORD(wParam) == BN_CLICKED) { //Info del doctor
					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

				}
				else if((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)){ //SALIR

					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaFinal, SW_SHOW);

				}
				else if ((LOWORD(wParam) == BTN_NUEVACITA && HIWORD(wParam) == BN_CLICKED)) { //Nueva Cita

			
					ShowWindow(hWnd, SW_HIDE);
					hVentanaNuevaCita = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita); //Creamos un nuevo Dialogo
					ShowWindow(hVentanaNuevaCita, SW_SHOW);

				}
				else if ((LOWORD(wParam) == IDCagenda_PAGAR && HIWORD(wParam) == BN_CLICKED)) {

			
					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaPago, SW_SHOW);

				}

				else if ((LOWORD(wParam) == IDC_AplicarFiltro && HIWORD(wParam) == BN_CLICKED)) {
					
					if (SI_HAY_CITAS == true) { //Si si hay citas entonces....

						FILTRO_APLICADO = true;

						HWND hFILTRO = GetDlgItem(hWnd, IDC_DATETIMEPICKER1);
						int longitud_Fecha_FILTRO = GetWindowTextLength(hFILTRO);
						char FECHA_Ingresado[15];


						GetWindowText(hFILTRO, FECHA_Ingresado, longitud_Fecha_FILTRO + 1);

						FECHA_FILTRO = FECHA_Ingresado; //Le pasamos a la variable string "FECHA_FILTRO" la fecha que almacenamos en el string

						DestroyWindow(hWnd);



						//Volvemos a crear la ventana para crear el filtro
						hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
						ShowWindow(hVentanaAgenda, SW_SHOW);


					}
					else { //Si no hay citas

						MessageBox(hWnd, "No hay fechas", "No hay fechas regsitradas aun", MB_OK);

					}
					
				}

				else if ((LOWORD(wParam) == IDC_CancelarFiltro && HIWORD(wParam) == BN_CLICKED)) {

					FILTRO_APLICADO = false;

					DestroyWindow(hWnd);

					//Volvemos a crear la ventana para crear el filtro
					hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
					ShowWindow(hVentanaAgenda, SW_SHOW);

				}
		
				break;

				case WM_CLOSE:
					break;

				case WM_DESTROY:
			
					break;

			default:
				break;
			}
			return FALSE;


		}

		BOOL CALLBACK FuncionVentanaInfoDoctor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {

				case WM_INITDIALOG: {

					if (doc != NULL) {
						//IMPRIMIMOS LA IMAFEN DEL DOCTOR
						HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
						HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen);
						SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
						

						HWND hlista = GetDlgItem(hWnd, IDC_Nombre1);
						SendMessage(hlista, LB_ADDSTRING, 0, (LPARAM)doc->nombreDoc); //Le mandamos lo ingresado en la casilla del doctor

						hlista = GetDlgItem(hWnd, IDC_Cedula1);
						SendMessage(hlista, LB_ADDSTRING, 0, (LPARAM)doc->cedula); //Le mandamos lo ingresado en la casilla de la cedula
					}
					

					aux = origen;

					

				}
				break;

				case WM_COMMAND:
			   {
			

					if (LOWORD(wParam) == IDC_Salir && HIWORD(wParam) == BN_CLICKED) {

								ShowWindow(hVentanaInfodelDoctor, SW_HIDE); //Ocultamos la ventana
								ShowWindow(hVentanaAgenda, SW_SHOW);
					}
					else if (LOWORD(wParam) == IDC_Modificar && HIWORD(wParam) == BN_CLICKED) {

						ShowWindow(hWnd,SW_HIDE);
						ShowWindow(hVentanaInfodelDoctorEDIT, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)) { //SALIR

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaFinal, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_NUEVACITA && HIWORD(wParam) == BN_CLICKED)) { //Nueva Cita

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaNuevaCita, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_AGENDA && HIWORD(wParam) == BN_CLICKED)) {

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaAgenda, SW_SHOW);

					}
		
		
			   }
				  break;

				case WM_CLOSE:
				break;

				case WM_DESTROY:
				break;

				default:
				break;
			}
			return FALSE;

		}

		BOOL CALLBACK FuncionVentanaNuevaCita(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	
			switch (msg) {

				case WM_INITDIALOG:{ //Para inicializar 

					SetTimer(hWnd, TM_RELOJ, 1000, NULL); //SET TIMER

					if (doc != NULL) {
						HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE);
						HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen2);
						SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
						SetDlgItemText(hWnd, IDC_Nomdoc, doc->nombreDoc);

						if (strlen(doc->nombreDoc) > 0) {

							SetDlgItemText(hWnd, IDC_DR, "Dr.");

						}
						else {

							SetDlgItemText(hWnd, IDC_DR, "");

						}

					}
					else {

						SetDlgItemText(hWnd, IDC_DR, "");
						SetDlgItemText(hWnd, IDC_Nomdoc, "");
					}
					

					

					HWND bt2 = GetDlgItem(hWnd, IDC_Agregar2);

					EnableWindow(bt2, false);

					HWND hCombo1;
					hCombo1 = GetDlgItem(hWnd, IDC_COMBOMASCOTA);                       //Lista desplegable 
					SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)"Perro");
					SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)"Gato");
					SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)"Conejo");
					SendMessage(hCombo1, CB_ADDSTRING, 0, (LPARAM)"Hamster");
					SendMessage(hCombo1, CB_SETCURSEL, 0, NULL);
			
				}
					break;

				case WM_COMMAND: {					

									if (LOWORD(wParam) == IDC_AgregarFoto && HIWORD(wParam) == BN_CLICKED) { //AGREGAR FOTO

										HWND bt2 = GetDlgItem(hWnd, IDC_Agregar2);

										HBITMAP h_Bmp;
										HWND h_id;

										OPENFILENAME ofn;
										ZeroMemory(&ofn, sizeof(ofn));
				

										ofn.hwndOwner = hWnd;
										ofn.lStructSize = sizeof(ofn);
										ofn.lpstrFile = chDirFile;
										ofn.nMaxFile = MAX_PATH;
										ofn.lpstrDefExt = "txt";
										ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
										ofn.lpstrFilter = "Bit Map Images (.bmp)\0.bmp\0All Files (.)\0*.*\0";

										if (GetOpenFileName(&ofn)) 
										{
											h_id = GetDlgItem(hWnd, IDC_imagen);
											h_Bmp = (HBITMAP)LoadImage(NULL, chDirFile, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
											SendMessage(h_id, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_Bmp);
											EnableWindow(bt2, true);
										}

									} //Fin del if (AGREGAR FOTO)

									else if (LOWORD(wParam) == IDC_Agregar2 && HIWORD(wParam) == BN_CLICKED) { //AGREGAR FOTO

										HBITMAP h_Bmp2;
										HWND h_id2;

										OPENFILENAME ofn;
										ZeroMemory(&ofn, sizeof(ofn));


										ofn.hwndOwner = hWnd;
										ofn.lStructSize = sizeof(ofn);
										ofn.lpstrFile = chDirFile2;
										ofn.nMaxFile = MAX_PATH;
										ofn.lpstrDefExt = "txt";
										ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
										ofn.lpstrFilter = "Bit Map Images (.bmp)\0.bmp\0All Files (.)\0*.*\0";

										if (GetOpenFileName(&ofn))
										{

											h_id2 = GetDlgItem(hWnd, IDC_foto2);
											h_Bmp2 = (HBITMAP)LoadImage(NULL, chDirFile2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
											SendMessage(h_id2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_Bmp2);
										}

									} //Fin del if (AGREGAR FOTO) 2


					else if (LOWORD(wParam) == IDnc_CANCEL && HIWORD(wParam) == BN_CLICKED) 
					{ //Boton Cancelar

										//VOLVEMOS A VACIAR ESTAS VARIABLES PARA QUE NO SE VAYA CON UNA DIRECCION ALMACENADA

										char chDirFile[MAX_PATH] = ""; //Donde almacenaremos la direccion de la imagen, en lo que validamos
										char chDirFile2[MAX_PATH] = ""; //Donde almacenaremos la direccion de la imagen, en lo que validamos

						DestroyWindow(hWnd); //Destruimos
						ShowWindow(hVentanaAgenda, SW_SHOW);

					}

					else if (LOWORD(wParam) == IDnc_Agendar && HIWORD(wParam) == BN_CLICKED){    //AGENDAR

										

						//VARIABLES IMPORTANTES
						int acumulador_validaciones=0;
						bool b_validado = false;
						bool identificador;
						aux = origen;
				
						identificador = ValidacionLongitud(hWnd); //Validamos el que no haya espacios vacios
				
						if (identificador == false) { //Si no esta todo relleno
							break;
						}

				
						char cValidaciondeDATOS[50];

						//Nombre
						HWND hEditBoxNombre = GetDlgItem(hWnd, IDCnc_Nombre); //Tomamos el ITEM
						int longitud_Nom = GetWindowTextLength(hEditBoxNombre); //Sacamos la longitud
						//identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght
				
						GetWindowText(hEditBoxNombre, cValidaciondeDATOS, longitud_Nom + 1);
						if (identificador == false) {
							MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
							break;
						}
						else {

							if (validar_nombre(cValidaciondeDATOS)==0) {

								MessageBox(hWnd, "PorFavor ingrese un nombre valido", "ERROR", MB_OK);
								break;
							}
							else {

								acumulador_validaciones = acumulador_validaciones + 1;

							}
						}

				
						//ESPECIE
						HWND hComboEspecie = GetDlgItem(hWnd, IDC_COMBOMASCOTA);
						int longitud_Especie = GetWindowTextLength(hComboEspecie);
						GetWindowText(hComboEspecie, cValidaciondeDATOS, longitud_Especie + 1);
						char Especie1[10]="Perro";
						char Especie2[10] = "Gato";
						char Especie3[10] = "Conejo";
						char Especie4[10] = "Hamster";
				
						if (identificador == false) {
							MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
							break;
						}
						else {
							if (validar_nombre(cValidaciondeDATOS) == 0 || strcmp(cValidaciondeDATOS,Especie1)!=0
								&& strcmp(cValidaciondeDATOS, Especie2) != 0 && strcmp(cValidaciondeDATOS, Especie3) != 0
									&& strcmp(cValidaciondeDATOS, Especie4) != 0) {

								MessageBox(hWnd, "PorFavor ingrese una especie valida", "ERROR", MB_OK);
								break;
							}
							else {

								acumulador_validaciones = acumulador_validaciones + 1;

							}
						}
					
					
				
				

												#pragma region VALIDACION_FECHA_Y_HORA

												//Hora

												HWND hHora = GetDlgItem(hWnd, IDC_DATETIMEPICKER2);
												int longitud_Hora = GetWindowTextLength(hHora);
										
												if (identificador == false) {
													MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
													break;
												}
												else {

													acumulador_validaciones = acumulador_validaciones + 1;

												}
				
												//Fecha
												char ch_Validacion_Fecha[30];
												char ch_Validacion_HORA[30];
												string str_FECHA_Ingresada;
												string str_FECHA_ACTUAL;
												string str_HORA_Ingresada;
												string str_HORA_ACTUAL;

												HWND hFecha = GetDlgItem(hWnd, IDC_DATETIME);
												int longitud_Fecha = GetWindowTextLength(hFecha);
										
												GetWindowText(hFecha, ch_Validacion_Fecha, longitud_Fecha + 1);
												GetWindowText(hHora, ch_Validacion_HORA, longitud_Hora + 1);

												str_FECHA_Ingresada = ch_Validacion_Fecha; //Igualamos la variable de fecha con lo almacenado en el char
												str_HORA_Ingresada = ch_Validacion_HORA; //ESTA ES LA HORA INGRESADA POR EL USUARIO

												if (identificador == false) {
													MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
													break;
												}
				
												else {

																			// FECHA_ACTUAL y HORA_ACTUAL son variables globales
																			str_FECHA_ACTUAL = FECHA_ACTUAL; //Igualamos el arreglo char a un string para poderlo mandar como parametro
																			str_HORA_ACTUAL = HORA_ACTUAL;

																			//Si la FECHA YA PASO 

																			if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 10) { //Todo salio bien

																				//Si llegamos a este punto las fechas y horas estan completamente validadas
																				acumulador_validaciones = acumulador_validaciones + 1;

																			}
																			else if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 4) { //Se necesita una hora de diferencia

																				MessageBox(hWnd, "Tiene una cita en esta misma fecha, pero cambie la hora, se necesita 1 hora de diferencia por cita", "ERROR", MB_OK);
																				break;

																			}
																			else if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 0) { //Esta cita se empalma con otra

																				MessageBox(hWnd, "Esta cita se empalma con otra", "ERROR", MB_OK);
																				break;
																	
																			}
																			else if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 1) { //Esta fecha ya paso

																				MessageBox(hWnd, "Esta fecha ya pasó", "ERROR", MB_OK);
																				break;

																			}

																			else if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 2) { //Esta ingresando el dia de hoy y la misma hora
																		
																				MessageBox(hWnd, "Esta ingresando el dia de hoy y la hora actual, haga una cita para mas tarde", "ERROR", MB_OK);
																				break;
																
																			}

																			else if (validar_fecha_y_hora(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada) == 3) { //Esta hora del dia de hoy ya paso

																				MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
																				break;

																			}
																	
																	
                             
												}
				
												#pragma endregion


						//Numero de Telefono
						HWND hEditBoxTelefono = GetDlgItem(hWnd, IDC_NumTel); //Tomamos el ITEM
						int longitud_Telefono = GetWindowTextLength(hEditBoxTelefono); //Sacamos la longitud

						//Mandamos el texto ingresado en la editbox a la variable de validacion
						GetWindowText(hEditBoxTelefono, cValidaciondeDATOS, longitud_Telefono + 1); 
				
						if (identificador == false) { //Si no todos los campos estan vacios

							MessageBox(hWnd, "Porfavor rellene los campos Vacios", "ERROR", MB_OK);
							break;

						}
						else {

							if (validar_numeroTEL(cValidaciondeDATOS) == 0) { //Si no es correcto

								MessageBox(hWnd, "Ingrese correctamente el numero de telefono", "ERROR", MB_OK);
								break;

							}
							else if(validar_numeroTEL(cValidaciondeDATOS) == 2) { //Si no son los digitos correctos

								MessageBox(hWnd, "Ingrese 8, 10 o 12 digitos en su numero de telefono", "ERROR", MB_OK);
								break;

							}
							else if (validar_numeroTEL(cValidaciondeDATOS) == 5) { //Si no son los digitos correctos

								MessageBox(hWnd, "Este numero ya fue ingresado en otra cita", "ERROR", MB_OK);
								break;

							}
							else {

								acumulador_validaciones = acumulador_validaciones + 1;

							}

						}

						//Nombre de mascota
						HWND hEditBoxNombreMascota = GetDlgItem(hWnd, IDC_NomMacota); //Tomamos el ITEM
						int longitud_nom_mascota = GetWindowTextLength(hEditBoxNombreMascota); //Sacamos la longitud
						GetWindowText(hEditBoxNombreMascota, cValidaciondeDATOS, longitud_nom_mascota + 1);
				
						if (identificador == false) {
							MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
							break;
						}
						else {

							if (validar_nombre(cValidaciondeDATOS) == 0) {

								MessageBox(hWnd, "PorFavor ingrese un nombre valido a su mascota", "ERROR", MB_OK);
								break;
							}
							else {

								acumulador_validaciones = acumulador_validaciones + 1;

							}
						}


						//Motivo
						HWND hEditBoxMotivo = GetDlgItem(hWnd, IDC_Motivo); //Tomamos el ITEM
						int longitud_Motivo = GetWindowTextLength(hEditBoxMotivo); //Sacamos la longitud
						//identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght
						//Verificamos si el usuario ingreso algo
				
						if (identificador == false) {
							MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
							break;
						}
						else {

							acumulador_validaciones = acumulador_validaciones + 1;

						}


						//Costo
						HWND hEditBoxCosto = GetDlgItem(hWnd, IDC_Costo); //Tomamos el ITEM
						int longitud_Costo = GetWindowTextLength(hEditBoxCosto); //Sacamos la longitud
						//identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght
						GetWindowText(hEditBoxCosto, cValidaciondeDATOS, longitud_Costo + 1);
				
						if (validar_costo(cValidaciondeDATOS) == 0) {

							MessageBox(hWnd, "PorFavor ingrese un costo valido", "ERROR", MB_OK);
							break;
						}
						else if (validar_costo(cValidaciondeDATOS) == 5) {

							MessageBox(hWnd, "El costo no puede pasar de las 6 cifras", "ERROR", MB_OK);
							break;
						}
						else {

							acumulador_validaciones = acumulador_validaciones + 1;

						}

										if (acumulador_validaciones == 8) {

											b_validado = true; //AQUI DEFINIMOS QUE TODO ESTA VALIDADO

										}
		
										if (b_validado == true) { //Si todo esta validado correctamente, ahora si pasamos la informacion a la estructura

											//Volvemos a buscar la ultima estructura, ya que alteramos la posicion de aux durante las validaciones
											if (origen == NULL) {

														origen = new Agenda;
														aux = origen;
														aux->prev = NULL; //NULL por que es la primera estructura, y no hay nada antes de ella

											}

											else {

												aux = origen;

												while (aux->next != NULL) { //Hasta que nos topemos con el final
													aux = aux->next;
												}

													aux->next = new Agenda;
													aux->next->prev = aux;
													aux = aux->next;

											}

											aux->next = NULL; //MARCAMOS ESTO PARA saber que es la ultima estructura

											GetWindowText(hEditBoxCosto, aux->Costo, longitud_Costo + 1);
											GetWindowText(hEditBoxMotivo, aux->Motivo, longitud_Motivo + 1);
											GetWindowText(hComboEspecie, aux->Especie, longitud_Especie + 1);
											GetWindowText(hHora, aux->Hora, longitud_Hora + 1);
											GetWindowText(hEditBoxNombre, aux->NombrePersona, longitud_Nom + 1);
											GetWindowText(hFecha, aux->Fecha, longitud_Fecha + 1);
											GetWindowText(hEditBoxTelefono, aux->Telefono, longitud_Telefono + 1);
											GetWindowText(hEditBoxNombreMascota, aux->NombreMascota, longitud_nom_mascota + 1);

											char vacio[] = " ";

											if (strcmp(chDirFile,vacio)!=0) { //Comparamos

												strcpy(aux->ch_DirFoto, chDirFile);
												strcpy(aux->ch_DirFoto2, chDirFile2);

											}

											char chDirFile[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos
											char chDirFile2[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos
										}

						aux->ID = globalID; //Lo igualamos la IDENTIFICADOR ACTUAL
						globalID++;
						#pragma endregion

						MessageBox(hWnd,"Se ha realizado la cita con exito", "Exito", MB_OK);

				
						DestroyWindow(hWnd);

						DestroyWindow(hVentanaAgenda);
						hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda); //Creamos la nueva ventana de agenda

						DestroyWindow(hVentanaNuevaCita);
						hVentanaNuevaCita = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita);
				
						ShowWindow(hVentanaAgenda, SW_SHOW);
			
					}

					//Demas botones

					else if (LOWORD(wParam) == BTN_INFODOC && HIWORD(wParam) == BN_CLICKED) {
						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)) { //SALIR

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaFinal, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_AGENDA && HIWORD(wParam) == BN_CLICKED)) {

					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaAgenda, SW_SHOW);

					}
			
				}
					break;

				case WM_TIMER:

				tlocal = localtime(&tiempo); //estructura donde obtengo el tiempo
				strftime(FECHA_ACTUAL, 11, "%d/%m/%Y", tlocal);//formato para guardar la fecha obtenido en *tlocal como dd/mm/yyyy

				strftime(HORA_ACTUAL, 11, "%H:%M:%S", tlocal); //EN FORMATO DE 24 HORAS

					break;


				case WM_CLOSE:
					KillTimer(hWnd, TM_RELOJ);
					break;

				case WM_DESTROY:
					break;

			

			}
			return FALSE;

		}

		BOOL CALLBACK FuncionVentanaPago(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {
				case WM_INITDIALOG: {

					aux = origen;

					while (aux->ID != globalBuscadorID) {
						aux = aux->next;
					}

					

					//IMPRIMIMOS IMAGEN Y NOMBRE
					
					
					if (doc != NULL) {
						HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE);
						HWND ID_DOC = GetDlgItem(hWnd, IDC_doctor);
						SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
						SetDlgItemText(hWnd, IDC_doc2, doc->nombreDoc);

						if (strlen(doc->nombreDoc) > 0) {

							SetDlgItemText(hWnd, IDC_DR, "Dr.");

						}
						else {

							SetDlgItemText(hWnd, IDC_DR, "");

						}
					}
					

					//Imprimimos la imagen

					HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);
		


					//IMPRESION EN TEXTOS ESTATICOS
					SetDlgItemText(hWnd, IDC_FECHA, aux->Fecha);
					SetDlgItemText(hWnd, IDC_HORA, aux->Hora);
					SetDlgItemText(hWnd, IDC_NOMBRE, aux->NombrePersona);
					SetDlgItemText(hWnd, IDC_TELEFONO, aux->Telefono);
					SetDlgItemText(hWnd, IDC_ESPECIE, aux->Especie);
					SetDlgItemText(hWnd, IDC_NOMBREmascota, aux->NombreMascota);
					SetDlgItemText(hWnd, IDC_Motivo, aux->Motivo);
					SetDlgItemText(hWnd, IDC_Costo, aux->Costo);

					#pragma region Desabilitar_botones
					HWND hbtnCONFIRMAR = GetDlgItem(hWnd, IDpago_Confirm);
					HWND hbtnCANCELAR = GetDlgItem(hWnd, IDpago_CANCEL); //En la list box

					EnableWindow(hbtnCONFIRMAR, false);
					EnableWindow(hbtnCANCELAR, true);    //Inahilitamos los botones
			
					#pragma endregion

					//CALCULANDO PRECIOS
					float preciof;
					float preciodecont;
					preciof = atof(aux->Costo) + (atof(aux->Costo)*.12); //Precio final con 12% mas de iva
					preciodecont = atof(aux->Costo);

					//IMPRIMIENDO LOS PRECIOS
					char Impresion[30];
					sprintf_s(Impresion, "%.4f", preciof);
					SetDlgItemText(hWnd, IDC_TOTAL, Impresion);
					sprintf_s(Impresion, "%.4f", preciodecont);
					SetDlgItemText(hWnd, IDC_Decontado, Impresion);
					sprintf_s(Impresion, "%.4f", preciof/3);
					SetDlgItemText(hWnd, IDC_3meses, Impresion);
					sprintf_s(Impresion, "%.4f", preciof/6);
					SetDlgItemText(hWnd, IDC_6meses, Impresion);
					sprintf_s(Impresion, "%.4f", preciof/12);
					SetDlgItemText(hWnd, IDC_12meses, Impresion);

				}
				  break;

				case WM_COMMAND:
					if (LOWORD(wParam) == IDpago_CANCEL && HIWORD(wParam) == BN_CLICKED) {

						DestroyWindow(hWnd);
						hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
						ShowWindow(hVentanaAgenda, SW_SHOW);

					}

					else if (LOWORD(wParam) == IDpago_Confirm && HIWORD(wParam) == BN_CLICKED) {

						//ELIMINAMOS TODO SOBRE ESA CITA

						if (aux->prev == NULL && aux->next == NULL) {
							delete aux;
							aux = origen = NULL;
						}
						else if (aux->next == NULL) {
							aux->prev->next = NULL;
							delete aux;
						}
						else if (aux->prev == NULL) {
							origen = origen->next;
							origen->prev = NULL;
							delete aux;
						}
						else {
							aux->next->prev = aux->prev;

							if (aux->prev != NULL) { //SI NO ESTAMOS EN EL ORIGEN

								aux->prev->next = aux->next;

							}

					
							delete aux;
						}
						aux = origen;


						DestroyWindow(hWnd);
						hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
						ShowWindow(hVentanaAgenda, SW_SHOW);
				
					}

					else if (LOWORD(wParam) == BTN_INFODOC && HIWORD(wParam) == BN_CLICKED) {
						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)) { //SALIR

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaFinal, SW_SHOW);

					}
					else if ((LOWORD(wParam) == BTN_AGENDA && HIWORD(wParam) == BN_CLICKED)) { //AGENDA

						DestroyWindow(hWnd);
						DestroyWindow(hVentanaAgenda);
						hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
						ShowWindow(hVentanaAgenda, SW_SHOW);

					}

					else if ((LOWORD(wParam) == BTN_NUEVACITA && HIWORD(wParam) == BN_CLICKED)) { //Nueva Cita

						ShowWindow(hWnd, SW_HIDE);
						ShowWindow(hVentanaNuevaCita, SW_SHOW);

					}


					//RADIOBUTTONS
					else if ((LOWORD(wParam) == IDC_RADIO1 && HIWORD(wParam) == BN_CLICKED)) { //

						#pragma region Desabilitar_botones
						HWND hbtnCONFIRMAR = GetDlgItem(hWnd, IDpago_Confirm);
						HWND hbtnCANCELAR = GetDlgItem(hWnd, IDpago_CANCEL); //En la list box

						EnableWindow(hbtnCONFIRMAR, true);
						EnableWindow(hbtnCANCELAR, true);    //Inahilitamos los botones

						#pragma endregion

					}
					else if ((LOWORD(wParam) == IDC_RADIO2 && HIWORD(wParam) == BN_CLICKED)) { //

				
						HWND hbtnCONFIRMAR = GetDlgItem(hWnd, IDpago_Confirm);
						HWND hbtnCANCELAR = GetDlgItem(hWnd, IDpago_CANCEL); //En la list box

						EnableWindow(hbtnCONFIRMAR, true);
						EnableWindow(hbtnCANCELAR, true);    //Inahilitamos los botones

				


					}
					else if ((LOWORD(wParam) == IDC_RADIO3 && HIWORD(wParam) == BN_CLICKED)) { //

						#pragma region Desabilitar_botones
						HWND hbtnCONFIRMAR = GetDlgItem(hWnd, IDpago_Confirm);
						HWND hbtnCANCELAR = GetDlgItem(hWnd, IDpago_CANCEL); //En la list box

						EnableWindow(hbtnCONFIRMAR, true);
						EnableWindow(hbtnCANCELAR, true);    //Inahilitamos los botones

						#pragma endregion

					}
					else if ((LOWORD(wParam) == IDC_RADIO4 && HIWORD(wParam) == BN_CLICKED)) { //

						#pragma region Desabilitar_botones
						HWND hbtnCONFIRMAR = GetDlgItem(hWnd, IDpago_Confirm);
						HWND hbtnCANCELAR = GetDlgItem(hWnd, IDpago_CANCEL); //En la list box
						HWND hRadio1 = GetDlgItem(hWnd, IDC_RADIO1);
						HWND hRadio2 = GetDlgItem(hWnd, IDC_RADIO2);
						HWND hRadio3 = GetDlgItem(hWnd, IDC_RADIO3);

						
						EnableWindow(hbtnCONFIRMAR, true);

						#pragma endregion

					}

					// FLECHAS

					else if (LOWORD(wParam) == IDC_izq && HIWORD(wParam) == BN_CLICKED) {

						HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
						HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
						SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);

					}
					else if (LOWORD(wParam) == IDC_der && HIWORD(wParam) == BN_CLICKED) {

						HBITMAP foto2 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto2, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
						HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
						SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto2);

					}


					break;

				case WM_CLOSE:
					break;

				case WM_DESTROY:
					break;

				default:
					break;
			}
			   return FALSE;


		}

		BOOL CALLBACK FuncionVentanaFINAL(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {
			case WM_INITDIALOG: {

				         
			     }
								break;

			case WM_COMMAND:

				if (LOWORD(wParam) == ID_Nosalir && HIWORD(wParam) == BN_CLICKED) {

					ShowWindow(hWnd, SW_HIDE);
					ShowWindow(hVentanaAgenda, SW_SHOW);

				}
				else if (LOWORD(wParam) == ID_Sisalir && HIWORD(wParam) == BN_CLICKED) {

			
					guardar(origen);

					if (doc != NULL) { //SI SI GUARDAMOS ALGO EN LA ESTRUCTURA DE DOCTOR SE GUARDA

						guardarDOC(doc);

					}

					DestroyWindow(hWnd);
					DestroyWindow(hVentanaAgenda);

				}

				break;

			case WM_CLOSE:
				break;

			case WM_DESTROY:
				PostQuitMessage(120);
				break;

			default:
				break;
			}

			return FALSE;


		}


		BOOL CALLBACK FuncionVentanaAgendaMostrar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {

				case WM_INITDIALOG: {

					aux = origen;
					 while (aux->ID != globalBuscadorID) {
						aux = aux->next;
					 }

					 //IMPRESION EN TEXTOS ESTATICOS
					SetDlgItemText(hWnd, IDC_FECHA, aux->Fecha);
					SetDlgItemText(hWnd, IDC_HORA, aux->Hora);
					SetDlgItemText(hWnd, IDC_NOMBRE, aux->NombrePersona);
					SetDlgItemText(hWnd, IDC_TELEFONO, aux->Telefono);
					SetDlgItemText(hWnd, IDC_ESPECIE, aux->Especie);
					SetDlgItemText(hWnd, IDC_NOMBREmascota, aux->NombreMascota);
					SetDlgItemText(hWnd, IDC_Motivo, aux->Motivo);
					SetDlgItemText(hWnd, IDC_Costo, aux->Costo);
	
					if (doc != NULL) {
						//IMPRIMIMOS LA INFORMACION DEL DOCTOR
						HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE);
						HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen);
						SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
						SetDlgItemText(hWnd, IDC_Nombredeldoctor, doc->nombreDoc);
					}
					


					//Imprimimos la imagen
		
					HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_foto2);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);
			
		
				}
								break;

			case WM_COMMAND:
				if (LOWORD(wParam) == IDC_REGRESAR1 && HIWORD(wParam) == MB_OK) {
					DestroyWindow(hWnd);
					hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
					ShowWindow(hVentanaAgenda, SW_SHOW);
				}
				else if (LOWORD(wParam) == IDC_izq && HIWORD(wParam) == MB_OK) {
			
					HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_foto2);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);

				}
				else if (LOWORD(wParam) == IDC_der && HIWORD(wParam) == MB_OK) {
		
					HBITMAP foto2 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_foto2);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto2);
		
				}
				break;

		

			case WM_CLOSE:
				break;

			case WM_DESTROY:
				break;

			default:
				break;
			}
			return FALSE;

		}

		BOOL CALLBACK FuncionVentanaAgendaModificar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
			switch (msg) {

				case WM_INITDIALOG: 
				{

					SetTimer(hWnd, TM_RELOJ, 1000, NULL); //SET TIMER
			
					aux = origen;
					while (aux->ID != globalBuscadorID) { //Buscamos que el ID coincida con el valor que se genera cuando clicamos la cita
						aux = aux->next;
					}

					if (aux->ch_DirFoto == " ") {

						HWND bt2 = GetDlgItem(hWnd, IDC_Agregar2);

						EnableWindow(bt2, false);

					}

					if (doc != NULL) {
						//IMPRIMIMOS LA INFORMACION DEL DOCTOR
						HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE);
						HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen);
						SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
						SetDlgItemText(hWnd, IDC_Nombredeldoctor, doc->nombreDoc);
					}
					else {
						SetDlgItemText(hWnd, IDC_Nombredeldoctor, "");

					}


					//Imprimimos las imagenes de las mascotas

					HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_foto1);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);

					HBITMAP foto2 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
					HWND ID2 = GetDlgItem(hWnd, IDC_foto2);
					SendMessage(ID2, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto2);


					HWND hEditBoxNombre = GetDlgItem(hWnd, IDCnc_Nombre);
					HWND hEditBoxTelefono = GetDlgItem(hWnd, IDC_NumTel);
					HWND hEditBoxMascota = GetDlgItem(hWnd, IDC_NomMacota);
					HWND hEditBoxMotivo = GetDlgItem(hWnd, IDC_Motivo);
					HWND hEditBoxCosto = GetDlgItem(hWnd, IDC_Costo);
					HWND hComboEspecie = GetDlgItem(hWnd, IDC_COMBOMASCOTA);
					HWND hTimePickerFecha = GetDlgItem(hWnd, IDC_DATETIME);
					HWND hTimePickerHora = GetDlgItem(hWnd, IDC_DATETIMEPICKER2);

					SetWindowText(hEditBoxNombre, aux->NombrePersona);
					SetWindowText(hEditBoxTelefono, aux->Telefono);
					SetWindowText(hEditBoxMascota, aux->NombreMascota);
					SetWindowText(hEditBoxMotivo, aux->Motivo);
					SetWindowText(hEditBoxCosto, aux->Costo);
					SetWindowText(hComboEspecie, aux->Especie);
					SetWindowText(hTimePickerFecha, aux->Fecha);
					SetWindowText(hTimePickerHora, aux->Hora);

										   //Lista desplegable 
					SendMessage(hComboEspecie, CB_ADDSTRING, 0, (LPARAM)"Perro");
					SendMessage(hComboEspecie, CB_ADDSTRING, 0, (LPARAM)"Gato");
					SendMessage(hComboEspecie, CB_ADDSTRING, 0, (LPARAM)"Conejo");
					SendMessage(hComboEspecie, CB_ADDSTRING, 0, (LPARAM)"Hamster");
					SendMessage(hComboEspecie, CB_ADDSTRING, 0, (LPARAM)"Otro");
					SendMessage(hComboEspecie, CB_SETCURSEL, 0, NULL);
				}
					break;

			   case WM_COMMAND: {
				  
									   if (LOWORD(wParam) == IDC_Cambiarfoto && HIWORD(wParam) == BN_CLICKED) { //AGREGAR FOTO

								  
										   HWND bt2 = GetDlgItem(hWnd, IDC_Agregar2);

										   HBITMAP h_Bmp;
										   HWND h_id;

										   OPENFILENAME ofn;
										   ZeroMemory(&ofn, sizeof(ofn));


										   ofn.hwndOwner = hWnd;
										   ofn.lStructSize = sizeof(ofn);
										   ofn.lpstrFile = chDirFile;
										   ofn.nMaxFile = MAX_PATH;
										   ofn.lpstrDefExt = "txt";
										   ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
										   ofn.lpstrFilter = "Bit Map Images (.bmp)\0.bmp\0All Files (.)\0*.*\0";

										   if (GetOpenFileName(&ofn))
										   {

											   h_id = GetDlgItem(hWnd, IDC_foto1);
											   h_Bmp = (HBITMAP)LoadImage(NULL, chDirFile, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
											   SendMessage(h_id, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_Bmp);

			
											   EnableWindow(bt2, true);
										   }

									   } //Fin del if (AGREGAR FOTO)

									   else if (LOWORD(wParam) == IDC_Cambiarfoto2 && HIWORD(wParam) == BN_CLICKED) { //AGREGAR FOTO

										   HBITMAP h_Bmp2;
										   HWND h_id2;

										   OPENFILENAME ofn;
										   ZeroMemory(&ofn, sizeof(ofn));


										   ofn.hwndOwner = hWnd;
										   ofn.lStructSize = sizeof(ofn);
										   ofn.lpstrFile = chDirFile2;
										   ofn.nMaxFile = MAX_PATH;
										   ofn.lpstrDefExt = "txt";
										   ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY | OFN_NOCHANGEDIR;
										   ofn.lpstrFilter = "Bit Map Images (.bmp)\0.bmp\0All Files (.)\0*.*\0";

										   if (GetOpenFileName(&ofn))
										   {

											   h_id2 = GetDlgItem(hWnd, IDC_foto2);
											   h_Bmp2 = (HBITMAP)LoadImage(NULL, chDirFile2, IMAGE_BITMAP, 150, 150, LR_LOADFROMFILE);
											   SendMessage(h_id2, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)h_Bmp2);
										   }

									   } //Fin del if (AGREGAR FOTO) 2

				   else if (LOWORD(wParam) == IDnc_CANCEL2 && HIWORD(wParam) == BN_CLICKED)
				   { //Boton Cancelar

										   //ESTAS VARIABLES LAS VOLVEMOS A VACIAR PARA QUE NO SE VAYAN CON UNA DIRECCION ALMACENADA

										   char chDirFile[MAX_PATH] = ""; //Donde almacenaremos la direccion de la imagen, en lo que validamos
										   char chDirFile2[MAX_PATH] = ""; //Donde almacenaremos la direccion de la imagen, en lo que validamos


					   DestroyWindow(hWnd); //Destruimos la ventana
					   hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda); //Creamos el dialogo
					   ShowWindow(hVentanaAgenda, SW_SHOW);

				   }

				   else if (LOWORD(wParam) == IDnc_CONFIRMAR && HIWORD(wParam) == BN_CLICKED) {    //AGENDAR

									//VARIABLES IMPORTANTES
					   int acumulador_validaciones = 0;
					   bool b_validado = false;
					   bool identificador;
					   aux = origen;

					   identificador = ValidacionLongitud(hWnd); //Validamos el que no haya espacios vacios

					   if (identificador == false) { //Si no esta todo relleno
						   break;
					   }



					   char cValidaciondeDATOS[50];

					   //Nombre
					   HWND hEditBoxNombre = GetDlgItem(hWnd, IDCnc_Nombre); //Tomamos el ITEM
					   int longitud_Nom = GetWindowTextLength(hEditBoxNombre); //Sacamos la longitud
					   //identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght

					   GetWindowText(hEditBoxNombre, cValidaciondeDATOS, longitud_Nom + 1);
					   if (identificador == false) {
						   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
						   break;
					   }
					   else {

						   if (validar_nombre(cValidaciondeDATOS) == 0) {

							   MessageBox(hWnd, "PorFavor ingrese un nombre valido", "ERROR", MB_OK);
							   break;
						   }
						   else {

							   acumulador_validaciones = acumulador_validaciones + 1;

						   }
					   }

					   //ESPECIE
					   HWND hComboEspecie = GetDlgItem(hWnd, IDC_COMBOMASCOTA);
					   int longitud_Especie = GetWindowTextLength(hComboEspecie);
					   GetWindowText(hComboEspecie, cValidaciondeDATOS, longitud_Especie + 1);

					   char Especie1[10] = "Perro";
					   char Especie2[10] = "Gato";
					   char Especie3[10] = "Conejo";
					   char Especie4[10] = "Hamster";

					   if (identificador == false) {
						   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
						   break;
					   }
					   else {
						   if (validar_nombre(cValidaciondeDATOS) == 0 || strcmp(cValidaciondeDATOS, Especie1) != 0
							   && strcmp(cValidaciondeDATOS, Especie2) != 0 && strcmp(cValidaciondeDATOS, Especie3) != 0
							   && strcmp(cValidaciondeDATOS, Especie4) != 0) {

							   MessageBox(hWnd, "PorFavor ingrese una especie valida", "ERROR", MB_OK);
							   break;
						   }
						   else {

							   acumulador_validaciones = acumulador_validaciones + 1;

						   }
					   }


									#pragma region VALIDACION_FECHA_Y_HORA

														   //Hora

														   HWND hHora = GetDlgItem(hWnd, IDC_DATETIMEPICKER2);
														   int longitud_Hora = GetWindowTextLength(hHora);

														   if (identificador == false) {
															   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
															   break;
														   }
														   else {

															   acumulador_validaciones = acumulador_validaciones + 1;

														   }

														   //Fecha
														   char ch_Validacion_Fecha[30];
														   char ch_Validacion_HORA[30];
														   string str_FECHA_Ingresada;
														   string str_FECHA_ACTUAL;
														   string str_HORA_Ingresada;
														   string str_HORA_ACTUAL;

														   HWND hFecha = GetDlgItem(hWnd, IDC_DATETIME);
														   int longitud_Fecha = GetWindowTextLength(hFecha);

														   GetWindowText(hFecha, ch_Validacion_Fecha, longitud_Fecha + 1);
														   GetWindowText(hHora, ch_Validacion_HORA, longitud_Hora + 1);

														   str_FECHA_Ingresada = ch_Validacion_Fecha; //Igualamos la variable de fecha con lo almacenado en el char
														   str_HORA_Ingresada = ch_Validacion_HORA; //ESTA ES LA HORA INGRESADA POR EL USUARIO

														   if (identificador == false) {
															   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
															   break;
														   }

														   else {

															   // FECHA_ACTUAL y HORA_ACTUAL son variables globales
															   str_FECHA_ACTUAL = FECHA_ACTUAL; //Igualamos el arreglo char a un string para poderlo mandar como parametro
															   str_HORA_ACTUAL = HORA_ACTUAL;

															   //Si la FECHA YA PASO 

															   if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada,aux->ID) == 10) { //Todo salio bien

																   //Si llegamos a este punto las fechas y horas estan completamente validadas
																   acumulador_validaciones = acumulador_validaciones + 1;

															   }
															   else if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada, aux->ID) == 4) { //Se necesita una hora de diferencia


																   MessageBox(hWnd, "Tiene una cita en esta misma fecha, pero cambie la hora, se necesita 1 hora de diferencia por cita", "ERROR", MB_OK);
																   break;

															   }
															   else if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada, aux->ID) == 0) { //Esta cita se empalma con otra

																   MessageBox(hWnd, "Esta cita se empalma con otra", "ERROR", MB_OK);
																   break;

															   }
															   else if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada, aux->ID) == 1) { //Esta fecha ya paso

																   MessageBox(hWnd, "Esta fecha ya pasó", "ERROR", MB_OK);
																   break;

															   }

															   else if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada, aux->ID) == 2) { //Esta ingresando el dia de hoy y la misma hora

																   MessageBox(hWnd, "Esta ingresando el dia de hoy y la hora actual, haga una cita para mas tarde", "ERROR", MB_OK);
																   break;

															   }

															   else if (validar_fecha_y_hora_CON_ID(FECHA_ACTUAL, HORA_ACTUAL, str_FECHA_Ingresada, str_HORA_Ingresada, aux->ID) == 3) { //Esta hora del dia de hoy ya paso

																   MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
																   break;

															   }



														   }
										   

									#pragma endregion


								//Numero de Telefono
								HWND hEditBoxTelefono = GetDlgItem(hWnd, IDC_NumTel); //Tomamos el ITEM
								int longitud_Telefono = GetWindowTextLength(hEditBoxTelefono); //Sacamos la longitud

								//Mandamos el texto ingresado en la editbox a la variable de validacion
								GetWindowText(hEditBoxTelefono, cValidaciondeDATOS, longitud_Telefono + 1);

								if (identificador == false) { //Si no todos los campos estan vacios

									MessageBox(hWnd, "Porfavor rellene los campos Vacios", "ERROR", MB_OK);
									break;

								}
								else {

									if (validar_numeroTEL(cValidaciondeDATOS) == 0) { //Si no es correcto

										MessageBox(hWnd, "Ingrese correctamente el numero de telefono", "ERROR", MB_OK);
										break;

									}
									else if (validar_numeroTEL(cValidaciondeDATOS) == 2) { //Si no son los digitos correctos

										MessageBox(hWnd, "Ingrese 8, 10 o 12 digitos en su numero de telefono", "ERROR", MB_OK);
										break;

									}
									else if (validar_numeroTEL(cValidaciondeDATOS) == 5) { //Si no son los digitos correctos

										MessageBox(hWnd, "Este numero ya fue ingresado en otra cita", "ERROR", MB_OK);
										break;

									}
									else {

										acumulador_validaciones = acumulador_validaciones + 1;

									}

								}

						

					   //Nombre de mascota
					   HWND hEditBoxNombreMascota = GetDlgItem(hWnd, IDC_NomMacota); //Tomamos el ITEM
					   int longitud_nom_mascota = GetWindowTextLength(hEditBoxNombreMascota); //Sacamos la longitud
					   GetWindowText(hEditBoxNombreMascota, cValidaciondeDATOS, longitud_nom_mascota + 1);

					   if (identificador == false) {
						   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
						   break;
					   }
					   else {

						   if (validar_nombre(cValidaciondeDATOS) == 0) {

							   MessageBox(hWnd, "PorFavor ingrese un nombre valido asu mascota", "ERROR", MB_OK);
							   break;
						   }
						   else {

							   acumulador_validaciones = acumulador_validaciones + 1;

						   }
					   }


					   //Motivo
					   HWND hEditBoxMotivo = GetDlgItem(hWnd, IDC_Motivo); //Tomamos el ITEM
					   int longitud_Motivo = GetWindowTextLength(hEditBoxMotivo); //Sacamos la longitud
					   //identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght
					   //Verificamos si el usuario ingreso algo

					   if (identificador == false) {
						   MessageBox(hWnd, "Porfavor Rellene los campos Vacios", "ERROR", MB_OK);
						   break;
					   }
					   else {

						   acumulador_validaciones = acumulador_validaciones + 1;

					   }


						//Costo
						HWND hEditBoxCosto = GetDlgItem(hWnd, IDC_Costo); //Tomamos el ITEM
						int longitud_Costo = GetWindowTextLength(hEditBoxCosto); //Sacamos la longitud
						//identificador = ValidacionLongitud(longitud);// "longitud" es el parametro lenght
						GetWindowText(hEditBoxCosto, cValidaciondeDATOS, longitud_Costo + 1);

						if (validar_costo(cValidaciondeDATOS) == 0) {

							MessageBox(hWnd, "PorFavor ingrese un costo valido", "ERROR", MB_OK);
							break;
						}
						else if (validar_costo(cValidaciondeDATOS) == 5) {

							MessageBox(hWnd, "El costo no puede pasar de las 6 cifras", "ERROR", MB_OK);
							break;
						}
						else {

							acumulador_validaciones = acumulador_validaciones + 1;

						}

													if (acumulador_validaciones == 8) {

														b_validado = true; //AQUI DEFINIMOS QUE TODO ESTA VALIDADO

													}

													if (b_validado == true) { //Si todo esta validado correctamente, ahora si pasamos la informacion a la estructura

														aux = origen;

														aux = origen;
														while (aux->ID != globalBuscadorID) { //Buscamos que el ID coincida con el valor que se genera cuando clicamos la cita
															aux = aux->next;
														}
														/*
														while (aux->next != NULL) { //Hasta que nos topemos con el final
															aux = aux->next;
														}
														*/

														GetWindowText(hEditBoxCosto, aux->Costo, longitud_Costo + 1);
														GetWindowText(hEditBoxMotivo, aux->Motivo, longitud_Motivo + 1);
														GetWindowText(hComboEspecie, aux->Especie, longitud_Especie + 1);
														GetWindowText(hHora, aux->Hora, longitud_Hora + 1);
														GetWindowText(hEditBoxNombre, aux->NombrePersona, longitud_Nom + 1);
														GetWindowText(hFecha, aux->Fecha, longitud_Fecha + 1);
														GetWindowText(hEditBoxTelefono, aux->Telefono, longitud_Telefono + 1);
														GetWindowText(hEditBoxNombreMascota, aux->NombreMascota, longitud_nom_mascota + 1);

														char vacio[] = " ";

														aux = origen;
														while (aux->ID != globalBuscadorID) { //Buscamos que el ID coincida con el valor que se genera cuando clicamos la cita
															aux = aux->next;
														}

																		if (strcmp(chDirFile, vacio) != 0 || strcmp(chDirFile2, vacio) != 0) {// SI la estructura ya tiene una primera imagen, lo mandamos a la variable que nos sirve como auxiliar

																			if (strcmp(chDirFile, vacio) != 0) {

																				strcpy(aux->ch_DirFoto, chDirFile);

																			}
																			

																			if(strcmp(chDirFile2, vacio) != 0){

																				strcpy(aux->ch_DirFoto2, chDirFile2);

                                                                            }


																		}
																	

														char chDirFile[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos
														char chDirFile2[MAX_PATH] = " "; //Donde almacenaremos la direccion de la imagen, en lo que validamos
													}
			   

						MessageBox(hWnd, "Se ha modificado la cita con exito", "Exito", MB_OK);


					   DestroyWindow(hWnd);

					   DestroyWindow(hVentanaAgenda);
					   hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda); //Creamos la nueva ventana de agenda

					   DestroyWindow(hVentanaNuevaCita);
					   hVentanaNuevaCita = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_NuevaCita), NULL, FuncionVentanaNuevaCita);

					   ShowWindow(hVentanaAgenda, SW_SHOW);

				   }

				   //Demas botones

				   else if (LOWORD(wParam) == BTN_INFODOC && HIWORD(wParam) == BN_CLICKED) {
					   ShowWindow(hWnd, SW_HIDE);
					   ShowWindow(hVentanaInfodelDoctor, SW_SHOW);

				   }
				   else if ((LOWORD(wParam) == BTN_SALIR && HIWORD(wParam) == BN_CLICKED)) { //SALIR

					   ShowWindow(hWnd, SW_HIDE);
					   ShowWindow(hVentanaFinal, SW_SHOW);

				   }
				   else if ((LOWORD(wParam) == BTN_AGENDA && HIWORD(wParam) == BN_CLICKED)) {

					   ShowWindow(hWnd, SW_HIDE);
					   ShowWindow(hVentanaAgenda, SW_SHOW);

				   }



			   }

							 break;

				case WM_TIMER:
					tlocal = localtime(&tiempo); //estructura donde obtengo el tiempo
					strftime(FECHA_ACTUAL, 11, "%d/%m/%Y", tlocal);//formato para guardar la fecha obtenido en *tlocal como dd/mm/yyyy

					strftime(HORA_ACTUAL, 11, "%H:%M:%S", tlocal); //EN FORMATO DE 24 HORAS

					break;

				case WM_CLOSE:
					KillTimer(hWnd, TM_RELOJ);
					break;

				case WM_DESTROY:
					break;

				default:
					break;
			}

			return FALSE;
		}

		BOOL CALLBACK FuncionVentanaAgendaEliminar(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {

			switch (msg) {

			  case WM_INITDIALOG: {

				aux = origen;

				while (aux->ID != globalBuscadorID) {
					aux = aux->next;
				}

				if (doc != NULL) {
					//IMPRIMIMOS LA INFORMACION DEL DOCTOR
					HBITMAP fotoDOC = (HBITMAP)LoadImage(NULL, doc->DirFileDoc, IMAGE_BITMAP, 80, 80, LR_LOADFROMFILE);
					HWND ID_DOC = GetDlgItem(hWnd, IDC_imagen3);
					SendMessage(ID_DOC, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)fotoDOC);
					SetDlgItemText(hWnd, IDC_Nombredeldoctor, doc->nombreDoc);
				}
				

				//Imprimimos la imagen
				HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
				HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
				SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);
		
				SetDlgItemText(hWnd, IDC_FECHA2, aux->Fecha);
				SetDlgItemText(hWnd, IDC_HORA2, aux->Hora);             //Damos la informacion del texto a los textos estaticos
				SetDlgItemText(hWnd, IDC_NOMBRE2, aux->NombrePersona);
				SetDlgItemText(hWnd, IDC_TELEFONO2, aux->Telefono);
				SetDlgItemText(hWnd, IDC_ESPECIE2, aux->Especie);
				SetDlgItemText(hWnd, IDC_NOMBREmascota2, aux->NombreMascota);
				SetDlgItemText(hWnd, IDC_Motivo2, aux->Motivo);
				SetDlgItemText(hWnd, IDC_Costo2, aux->Costo);
			  }
				break;

			  case WM_COMMAND: {

		

				if (LOWORD(wParam) == IDCANCEL && HIWORD(wParam) == BN_CLICKED) { //CANCELAR
					DestroyWindow(hWnd);
					hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
					ShowWindow(hVentanaAgenda, SW_SHOW);
				}
				else if (LOWORD(wParam) == IDCelim_ELIMINAR && HIWORD(wParam) == BN_CLICKED) { //ELIMINAR
			
					if (aux->prev == NULL && aux->next == NULL) {
						delete aux;
						aux = origen = NULL;
					}
					else if (aux->next == NULL) {
						aux->prev->next = NULL;
						delete aux;
					}
					else if (aux->prev == NULL) {
						origen = origen->next;
						origen->prev = NULL;
						delete aux;
					}
					else {
						aux->next->prev = aux->prev;
						aux->prev->next = aux->next;
						delete aux;
					}
					aux = origen;
			

					DestroyWindow(hWnd);
					hVentanaAgenda = CreateDialog(hGlobalInstance, MAKEINTRESOURCE(IDD_Agenda), NULL, FuncionVentanaAgenda);
					ShowWindow(hVentanaAgenda, SW_SHOW);
				}

				else if (LOWORD(wParam) == IDC_izq && HIWORD(wParam) == BN_CLICKED) {

					HBITMAP foto1 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto1);

				}
				else if (LOWORD(wParam) == IDC_der && HIWORD(wParam) == BN_CLICKED) {

					HBITMAP foto2 = (HBITMAP)LoadImage(NULL, aux->ch_DirFoto2, IMAGE_BITMAP, 120, 120, LR_LOADFROMFILE);
					HWND ID1 = GetDlgItem(hWnd, IDC_imagen);
					SendMessage(ID1, STM_SETIMAGE, (WPARAM)IMAGE_BITMAP, (LPARAM)foto2);

				}

			  }
				break;

				case WM_CLOSE:
					break;

				case WM_DESTROY:
					break;

				default:
					break;
			}

			return FALSE;

		}


#pragma endregion




//PENDIENTE INICIO DE SESION

//VALIDACIONES

#pragma region VALIDACIONES

		void Ordenar_estructuras() {

			#pragma region ORDENAMOS_LAS_ESTRUCTURAS

			char vacio[1] = "";

			aux_para_ordenar_lista = new aux_para_ordenar;
			aux = origen;

			//Contamos cuantas estructuras tenemos
			int Cant_de_estructuras = 0;

			while (aux != NULL) {

				Cant_de_estructuras = Cant_de_estructuras + 1;

				aux = aux->next;

			}

			aux = origen;

			if (aux!=NULL && aux->next!=NULL) { //Si tenemos mas de una estrcurura
				//METODO BURBUJA
				

				for (int i = 0; i < Cant_de_estructuras; i++) {

					for (int j = 0; j < Cant_de_estructuras; j++) {

						if (aux->next!= NULL && validar_fecha(aux->Fecha, aux->next->Fecha) == 5) {

							//Copiamos todo en la estrcutura auxiliar
							strcpy(aux_para_ordenar_lista->Fecha,aux->Fecha);
							strcpy(aux_para_ordenar_lista->Hora,aux->Hora);
							strcpy(aux_para_ordenar_lista->NombrePersona,aux->NombrePersona);
							strcpy(aux_para_ordenar_lista->NombreMascota,aux->NombreMascota);
							strcpy(aux_para_ordenar_lista->Telefono,aux->Telefono);
							strcpy(aux_para_ordenar_lista->Especie,aux->Especie);
							strcpy(aux_para_ordenar_lista->Costo,aux->Costo);
							strcpy(aux_para_ordenar_lista->Motivo,aux->Motivo);

							strcpy(aux_para_ordenar_lista->ch_DirFoto, aux->ch_DirFoto);
							strcpy(aux_para_ordenar_lista->ch_DirFoto2, aux->ch_DirFoto2);

							aux_para_ordenar_lista->ID = aux->next->ID;

							//Intercambiamos lugar
							strcpy(aux->Fecha,aux->next->Fecha);
							strcpy(aux->Hora,aux->next->Hora);
							strcpy(aux->NombrePersona,aux->next->NombrePersona);
							strcpy(aux->NombreMascota,aux->next->NombreMascota);
							strcpy(aux->Telefono,aux->next->Telefono);
							strcpy(aux->Especie,aux->next->Especie);
							strcpy(aux->Costo,aux->next->Costo);
							strcpy(aux->Motivo,aux->next->Motivo);

							strcpy(aux->ch_DirFoto, aux->next->ch_DirFoto);
							strcpy(aux->ch_DirFoto2, aux->next->ch_DirFoto2);

							aux->next->ID = aux->ID;


							//Le damos la informacion a la estructura que acabamos de acmabiar
							strcpy(aux->next->Fecha,aux_para_ordenar_lista->Fecha);
							strcpy(aux->next->Hora,aux_para_ordenar_lista->Hora);
							strcpy(aux->next->NombrePersona,aux_para_ordenar_lista->NombrePersona);
							strcpy(aux->next->NombreMascota,aux_para_ordenar_lista->NombreMascota);
							strcpy(aux->next->Telefono,aux_para_ordenar_lista->Telefono);
							strcpy(aux->next->Especie,aux_para_ordenar_lista->Especie);
							strcpy(aux->next->Costo,aux_para_ordenar_lista->Costo);
							strcpy(aux->next->Motivo,aux_para_ordenar_lista->Motivo);
							strcpy(aux->next->ch_DirFoto,aux_para_ordenar_lista->ch_DirFoto);
							strcpy(aux->next->ch_DirFoto2,aux_para_ordenar_lista->ch_DirFoto2);
							aux->next->ID = aux_para_ordenar_lista->ID;

							

						}

						aux = aux->next;
						/*
						if (aux->next != NULL) {
							aux = aux->next;
							aux = aux;
						
						}
						*/

					}

					aux = origen;

					
				}

			}
					#pragma endregion

			delete aux_para_ordenar_lista;


		}

		int ValidacionLongitud(HWND hWnd) {

			//Variables para ver lo que esta en las casillas
			int longitudNombre;
			int longitudTelefono;
			int longitudMascota;
			int longitudMotivo;
			int longitudCosto;
			int longitudEspecie;
			int longitudFecha;
			int longitudHora;

			bool identificador = true;

			HWND hEditBoxNombre = GetDlgItem(hWnd, IDCnc_Nombre);
			longitudNombre = GetWindowTextLength(hEditBoxNombre);

			HWND hEditBoxTelefono = GetDlgItem(hWnd, IDC_NumTel);
			longitudTelefono = GetWindowTextLength(hEditBoxTelefono);

			HWND hEditBoxMascota = GetDlgItem(hWnd, IDC_NomMacota);
			longitudMascota = GetWindowTextLength(hEditBoxMascota);

			HWND hEditBoxMotivo = GetDlgItem(hWnd, IDC_Motivo);
			longitudMotivo = GetWindowTextLength(hEditBoxMotivo);

			HWND hEditBoxCosto = GetDlgItem(hWnd, IDC_Costo);
			longitudCosto = GetWindowTextLength(hEditBoxCosto);

			HWND hComboEspecie = GetDlgItem(hWnd, IDC_COMBOMASCOTA);
			longitudEspecie = GetWindowTextLength(hComboEspecie);

			HWND hTimePickerFecha = GetDlgItem(hWnd, IDC_DATETIME);
			longitudFecha = GetWindowTextLength(hTimePickerFecha);

			HWND hTimePickerHora = GetDlgItem(hWnd, IDC_DATETIMEPICKER2);
			longitudHora = GetWindowTextLength(hTimePickerHora);

			if (longitudNombre <= 0 || longitudTelefono <= 0 || longitudMascota <= 0 || longitudMotivo <= 0 || longitudCosto <= 0 || longitudEspecie <= 0 || longitudFecha <= 0 || longitudHora <= 0) {
				MessageBox(hVentanaNuevaCita, "Porfavor rellene los espacios vacios", "ERROR", MB_OK);
				return false;
			}
			else {

				return true;
			}
		}

		int ValidacionLongitudDOC(HWND hWnd) { //Validacion de longitud

			//Variables para ver lo que esta en las casillas
			int longitudNombre;
			int longitudCedula;

			bool identificador = true;

			HWND hEditBoxNombre = GetDlgItem(hWnd, IDC_EDIT1);
			longitudNombre = GetWindowTextLength(hEditBoxNombre);

			HWND hEditBoxTelefono = GetDlgItem(hWnd, IDC_EDIT2);
			longitudCedula = GetWindowTextLength(hEditBoxTelefono);



			if (longitudNombre <= 0 || longitudCedula <= 0) {

				MessageBox(hVentanaNuevaCita, "Porfavor rellene los espacios vacios", "ERROR", MB_OK);
				return false;
			}
			else {

				return true;
			}
		} //Validar si hay algo ingresado


		int INICIOSESION(char LogUsuario[] ,char LogContraseña[] ) {

			ifstream lectura("Datos.txt", ios::in);//Abrimos el archivo en modo lectura

			if (lectura.fail()) {

				MessageBox(NULL, "ERROR", "No se ha podido realizar la lectura", MB_OK);
				return 0;
			}

			else {

				while (!lectura.eof()) {  //En este ciclo leemos todo el documento de texto

					lectura >> Usuario;
					lectura >> Contraseña;

					if (strcmp(LogUsuario, Usuario) == 0 && strcmp(LogContraseña, Contraseña) == 0) {

						return 1; //La contraseña es correcta

					}
					else {

				
						return 0;

					}

				}

			}

		}

		int validar_numeroTEL(char numero[])////////FUNCION PARA VALIDAR QUE SOLO SE INGRESEN NUMEROS///////////////////////
		{
			int i;    //POSICION


			if (strlen(numero) != 8 && strlen(numero) != 10 && strlen(numero) != 12) {

				return 2;

			}

			for (i = 0; i < strlen(numero); i++)
			{
				if (numero[i] == 45) //El numero 45 es el signo de negativo
				{
					return 0;
				}

				if (numero[i] == 46)//46 en el codigo ASCI es el punto "."
				{
					return 0;
				}

				if (!isdigit(numero[i]))//El numero 46 es un punto //El isdigit te da VERDADERO si es numero, en este caso si NO ES NUMERO hace lo siguiente
				{
						return 0;
				}

			}


			return 1;//Si se retorna 1 significa que la cadena ingresada solo contiene numeros
		}

		int validar_nombre(char nombre[])
		{

			int i;
			char espacio[2] = " ";

			if (strlen(nombre) == 0) { //No ingreso ningun caracter

				return 0;

			}

			for (i = 0; i < strlen(nombre); i++) // SRLEN te da el numero de caracteres que contiene la cadena "nombre"
			{
				if(!(nombre[i] >= 65 && nombre[i] <= 90 
					|| nombre[i] >= 97 && nombre[i] <= 122 
					|| nombre[i]==espacio[0] || nombre[i] >= 129 && nombre[i] <= 154 
					|| nombre[i] >= 160 && nombre[i] <= 165||nombre[i]==-19||nombre[i]==46
					||nombre[i]==-23 || nombre[i] == -55 || nombre[i] == -31 || nombre[i] == -13 
					|| nombre[i] == -6 || nombre[i] == -63 || nombre[i] == -51 || nombre[i] == -45 
					|| nombre[i] == -38)) // del 65 al 90 son letras mayusculas, y del 97 al 122 son letras minusculas
				{

					return 0; //Si el usuario NO solo ingreso letras se retornara un 0
				}

			}
			return 1;//Si el usuario ingreso solo letras entonces se retorna el 1

		}

		int validar_costo(char costo[])////////FUNCION PARA VALIDAR QUE SOLO SE INGRESEN NUMEROS///////////////////////
		{
			int contador_de_puntos = 0;
			int i;//POSICION
			int despues_del_punto = -1;//Entero para contar las posiciones despues de un punto

			if (strlen(costo) > 6) {

				return 5;

			}

			for (i = 0; i < strlen(costo); i++)
			{
				if (costo[i] == 45) //El numero 45 es el signo de negativo
				{

					return 0;

				}

				if (costo[i] == 46)//46 en el codigo ASCI es el punto "."
				{
					contador_de_puntos = contador_de_puntos + 1;
					//Aqui vamos contando los puntos que tiene el numero ingresado

				}

				if (contador_de_puntos == 1) {//Si tenemos un punto decimal

					despues_del_punto = despues_del_punto + 1; //Aqui le sumamos 1 de una posicion

					if (despues_del_punto > 2)//SI despues de identificar un punto hay mas de dos posiciones, osea mas de 2 decimales de dice lo siguiente
					{
						return 0;

					}

				}

				if (contador_de_puntos == 2)
				{
					return 0;
				}

				if (!isdigit(costo[i]))//El numero 46 es un punto //El isdigit te da VERDADERO si es numero, en este caso si NO ES NUMERO hace lo siguiente
				{
					int o = 1;

					if (costo[i] == 46) {//Si el caracter de la posicion i es un punto, igualamos la o a 0 para que NO se lo niegue

						o = 0;
					}

					if (o == 1) // la o nos indica cuantos puntos ha ingresado el usuario, en caso de aplicar mas de 1 punto entonces
					{
						return 0;

					}
				}
			}
			return 1;//Si se retorna 1 significa que la cadena ingresada solo contiene numeros
		}


		void guardar(Agenda *origen) {

			ofstream miArchivo;

			miArchivo.open("Informaciondelaagenda.bin", ios::out | ios::binary | ios::trunc);
	      
			if (!miArchivo.is_open()) { //Si hubo un fallo al abrirlo
		
				return;
			}
			
			Agenda *t = origen;

			while (t != NULL) {

				miArchivo.write(reinterpret_cast <char*>(t), sizeof(Agenda));
				t = t->next;
			}
			//Cambiamos nuestra estructura a un puntero de char del tamaño en bytes del mismo usuario.
  
			miArchivo.close();

			MessageBox(NULL, "La información se ha guardado exitosamente", "Guardado Exitoso", MB_OK);

			return;

		}

		void cargar() {
	
			ifstream miArchivo;

			miArchivo.open("Informaciondelaagenda.bin", ios::in | ios::binary | ios::ate);
			/*
			  La propiedad ate nos situa al ultimo caracter escrito
			*/

			if (!miArchivo.is_open()) {
				MessageBox(NULL, "ERROR", "No se encontro ningun archivo", MB_OK);
				return;
			}

			int size = miArchivo.tellg(); //Esto nos dira la cantidad de elementos en el archivo

			if (size == 0) { /*Como nos pusimos al final si este nos devuelve 0 entonces no hay info*/
				MessageBox(NULL, "Vacio", "No hay datos guardados", MB_OK);
				return;
			}

			for (int i = 0; i < (size / sizeof(Agenda)); i++) {

				#pragma region NOTAS

				/*La formula size/sizeof(Agenda), seran la cantidad de veces que el ciclo se repetira

				  Ej:    La estructura AGENDA pesa 20 bytes.
						 Si tenemos 3 estructuras, esto indica que en el archivo se escribieron 60 caracteres.
						 con tellg nos devolvera 60. sizeof nos dice el tamaño de la estructura.

						 60/20 = 3
						  3 Pacientes en total.

				*/
				#pragma endregion
				

				if (origen == NULL) {
					origen = new Agenda;
					aux = origen;
					aux->prev = NULL;
				}
				else {
					while (aux->next != NULL)
						aux = aux->next;
					aux->next = new Agenda;
					aux->next->prev = aux;
					aux = aux->next;
				}

				/*Flujo de lectura*/
				Agenda *T = new Agenda; //Agenda Temporal
				

				miArchivo.seekg(i * sizeof(Agenda));

				#pragma region NOTAS
				/*
							  seekg nos permite movernos en una posicion indicada en el archivo.
							  Por ejemplo, si ponemos 0 nos posicionara en el inicio del archivo.
							  Aqui usaremos la variable i que nos indica que iteracion vamos para
							  movernos en el numero de alumnos:

								0*20 = 0 :::: Nos posiciona al inicio del archivo en el primer paciente.
								1*20 = 20 ::: Nos posiciona en el segundo paciente.
								2*20 = 40 ::: Nos posiciona en el ultimo paciente.

								Esto debido a que cada paciente escribio un total de 20 caracteres y el
								siguiente paciente empieza donde el ultimo acabo.
				*/

				#pragma endregion
			


				miArchivo.read(reinterpret_cast<char*>(T), sizeof(Agenda));
				#pragma region NOTAS

				/*
				  read hace lo opuesto al write, este va a leer una cantidad asignada y la depositara en
				  una variable como primer parametro. El problema radica en que Temp no es una cadena de
				  caracteres y la informacion en el archivo SI es una cadena de caracteres debemos realizar
				  una interpretacion de esos datos.
				  _______
				  |     |
				  |     |  <--- Esto es un espacio de memoria en el Heap de un Paciente generado con new
				  |     |
				  -------

				  Al nosotros hacer un reinterpret la informacion que se guarde en el HEAP ya no sera de un
				  Paciente, si no de un "Paciente" obtenido de una cadena de caracteres.

				  mmmmm
				  s   s
				  s   s    <---- Esta es la interpretacion de un Paciente basado en una cadena.
				  s   s
				  w w w

				  No son lo mismo...
				  Por lo tanto al ejecutar un delete no va a saber si esta borrando un "Paciente" o un Paciente.
				  Esto lo tenemos que indicar en el delete pero para evitarnos problemas el TEMPORAL es el unico
				  que se convertira en "Paciente", extraemos los datos a un Paciente real y borramos el "Paciente"
				  temporal.

				*/

				#pragma endregion

				

				strcpy(aux->NombrePersona ,T->NombrePersona);
				strcpy(aux->Telefono,T->Telefono);
				strcpy(aux->Fecha, T->Fecha);
				strcpy(aux->Hora, T->Hora);
				strcpy(aux->Especie, T->Especie);
				strcpy(aux->NombreMascota, T->NombreMascota);
				strcpy(aux->Motivo, T->Motivo);
				strcpy(aux->Costo, T->Costo);
				strcpy(aux->ch_DirFoto, T->ch_DirFoto);
				strcpy(aux->ch_DirFoto2, T->ch_DirFoto2);
		
					/*Extraemos la informacion del temporal*/

					delete reinterpret_cast<char *>(T);
				/*
				  La unica manera de borrar esta interpretacion de datos es decirle al delete
				  que realmente borre una interpretacion de datos.
				*/

				aux->next = NULL;
				aux = origen;
			}
			miArchivo.close();
			MessageBox(NULL, "Exito", "Cargado exitoso", MB_OK);
		}

		void cargarDOC() {

			ifstream miArchivoDOC;

			miArchivoDOC.open("InformaciondelDOCTOR.bin", ios::in | ios::binary | ios::ate);

			int size = miArchivoDOC.tellg();

			if (size == 0) { //Si no arroja 0 significa que no hay informacion

				return;
			}
			for (int i = 0; i < (size / sizeof(DOCTOR)); i++) {

				if (doc == NULL) {
					doc = new DOCTOR;
				}

				DOCTOR *temporal = new DOCTOR;
				//Generamos un DOCTOR temporal en el heap

				miArchivoDOC.seekg(i * sizeof(DOCTOR));

				miArchivoDOC.read(reinterpret_cast<char*>(temporal), sizeof(DOCTOR));

				strcpy(doc->nombreDoc,temporal->nombreDoc);
				strcpy(doc->cedula, temporal->cedula);
				strcpy(doc->DirFileDoc, temporal->DirFileDoc);

				delete reinterpret_cast<char*>(temporal);

			}

			miArchivoDOC.close();
			//CRGADO EXITOSO
		}

		void guardarDOC(DOCTOR*) {

			ofstream miArchivoDOC;

			miArchivoDOC.open("InformaciondelDOCTOR.bin", ios::out | ios::binary | ios::trunc);

			if (!miArchivoDOC.is_open()) { //Si hubo un fallo al abrirlo

				return;
			}

			DOCTOR *t = doc;

          	miArchivoDOC.write(reinterpret_cast <char*>(t), sizeof(DOCTOR));
				
			
			//Cambiamos nuestra estructura a un puntero de char del tamaño en bytes del mismo usuario.

			miArchivoDOC.close();

			MessageBox(NULL, "La información se ha guardado exitosamente", "Guardado Exitoso", MB_OK);

			return;


		}

		
		//Validacion de fecha y hora

		int validar_fecha_y_hora(string FECHA_ACTUAL, string HORA_ACTUAL, string FECHA_INGRESADA, string HORA_INGRESADA) {

			if (validar_fecha(FECHA_INGRESADA, FECHA_ACTUAL) == 0) { //SI YA PASO

				return 1;

			}

			else if (validar_fecha(FECHA_INGRESADA, FECHA_ACTUAL) == 2) { //Si estamos en el mismo dia

				if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 0) { //Mandamos a llamar la funcion para validar que no se empalme

					return 2;
					/*
					MessageBox(hWnd, "Esta ingresando el dia de hoy y la hora actual, haga una cita para mas tarde", "ERROR", MB_OK);
					break;
					*/
				}

				else if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 2) { //YA PASO LA HORA DE HOY

					return 3;
					/*
					MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
					break;
					*/
				}

				else if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 3) { //YA PASO LA HORA DE HOY

					return 3;
					/*
					MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
					break;
					*/
				}
			}

			if (validar_fecha2(FECHA_INGRESADA, HORA_INGRESADA) == 0) { //Alguna fecha se empalma

				return 0;

			}
			else if(validar_fecha2(FECHA_INGRESADA, HORA_INGRESADA) == 4) { //No hay una diferencia de 1 hora al menos

				return 4;

			}
			else if (validar_fecha2(FECHA_INGRESADA, HORA_INGRESADA) == 10) { //Todo salio correctamente

				return 10;

			}




		}

		int validar_fecha2(string FECHA_INGRESADA, string HORA_INGRESADA) {

			//Despues de validar lo basico en la primera funcion de validar fecha, pasamos a comparar las demas fechas de las otras citas

			if (origen != NULL && origen->next != NULL) { //Si ya tenemos mas de 2 estructuras

				string HORA1;
				string HORA2;

				string FECHA1;
				string FECHA2;

				aux = origen;
				FECHA1 = aux->Fecha;
				HORA1 = aux->Hora;

				FECHA2 = aux->next->Fecha;
				HORA2 = aux->next->Hora;

				int Posicion_FECHA1 = 0;

#pragma region ORDENAMOS_LAS_ESTRUCTURAS

				aux = origen;

				//Contamos cuantas estructuras tenemos
				int Cant_de_estructuras = 0;

				while (aux != NULL) {

					Cant_de_estructuras = Cant_de_estructuras + 1;

					aux = aux->next;

				}

				aux = origen;

				if (aux != NULL && aux->next != NULL) { //Si tenemos mas de una estrcurura
					//METODO BURBUJA


					for (int i = 0; i < Cant_de_estructuras; i++) {

						for (int j = 0; j < Cant_de_estructuras; j++) {

							if (aux->next != NULL && validar_fecha(aux->Fecha, aux->next->Fecha) == 2) { //Si estamos en al misma fecha || La ultima condicion es para saber si no tienen el msimo ID


									//Pasamos ahora a comparar las horas de dichas fechas, para corroborar que no se empalman
								HORA2 = aux->next->Hora;
								HORA1 = aux->Hora;

								if (validar_hora_FORMATO_12(HORA1, HORA2) == 0) {

									/*
									Mandamos a llamar la funcion con formato de 12 horas para converirlo a formato de 24 y comparar las horas
									Si son iguales las horas, se empalma la cita

									*/


									return 0; //Se empalma

								}
								else if (validar_hora_FORMATO_12(HORA1, HORA2) == 4) {

									return 4; //NO HAY UNA DIFERENCIA DE AL MENOS 1 HORA ENTRE CADA CITA

								}


							}

							aux = aux->next;
							/*
							if (aux->next != NULL) {
								aux = aux->next;
								aux = aux;

							}
							*/

						}

						aux = origen;


					}

				}

#pragma endregion

				return 10;

			}

			//En caso de ser apenas la segunda cita

			else if (origen != NULL && origen->next == NULL) { //Aqui vamos a comparar la hora ya registrada, con la hora

				string HORA1;
				string HORA2;

				string FECHA1;
				string FECHA2;

				//Igualamos las fechas que vamos a comparar para poderlas mandar como parametro
				FECHA2 = origen->Fecha;
				FECHA1 = FECHA_INGRESADA;


				//ESTO es para ver si la cita del origen no es igual a la cita de la 2da estructura


				if (validar_fecha(FECHA1, FECHA2) == 2) { //Cuando retorna 2 significa que estas citas estan en el mismo dia

					//Pasamos ahora a comparar las horas de dichas fechas, para corroborar que no se empalman
					HORA2 = origen->Hora;
					HORA1 = HORA_INGRESADA;

					if (validar_hora_FORMATO_12(HORA1, HORA2) == 0) {

						/*
						Mandamos a llamar la funcion con formato de 12 horas para converirlo a formato de 24 y comparar las horas
						Si son iguales las horas, se empalma la cita

						*/


						return 0; //Se empalma

					}
					else if (validar_hora_FORMATO_12(HORA1, HORA2) == 4) {

						return 4; //NO HAY UNA DIFERENCIA DE AL MENOS 1 HORA ENTRE CADA CITA

					}
					else if (validar_hora_FORMATO_12(HORA1, HORA2) == 10) {

						return 10; //Todo salio bien

					}


				}

				else { //Si ninguna fecha se topa, entonces no hay citas que se empalmen

					return 10;
				}
			}

			else if (origen == NULL) { //Si origen es igual NULL, no tenemos que comparar la fecha y hora que ninguna otra

				return 10; //Retornamos este numero, de que hemos terminado de validar

			}

		}

		int validar_fecha_y_hora_CON_ID(string FECHA_ACTUAL, string HORA_ACTUAL, string FECHA_INGRESADA, string HORA_INGRESADA,int ID) {

			if (validar_fecha(FECHA_INGRESADA, FECHA_ACTUAL) == 0) { //SI YA PASO

				return 1;

			}

			else if (validar_fecha(FECHA_INGRESADA, FECHA_ACTUAL) == 2) { //Si estamos en el mismo dia

				if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 0) { //Mandamos a llamar la funcion para validar que no se empalme

					return 2;
					/*
					MessageBox(hWnd, "Esta ingresando el dia de hoy y la hora actual, haga una cita para mas tarde", "ERROR", MB_OK);
					break;
					*/
				}

				else if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 2) { //YA PASO LA HORA DE HOY

					return 3;
					/*
					MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
					break;
					*/
				}

				else if (validar_hora(HORA_INGRESADA, HORA_ACTUAL) == 3) { //YA PASO LA HORA DE HOY

					return 3;
					/*
					MessageBox(hWnd, "Esta hora del dia de HOY ya pasó", "ERROR", MB_OK);
					break;
					*/
				}
			}

			if (validar_fecha2_CON_ID(FECHA_INGRESADA, HORA_INGRESADA, ID) == 0) { //Alguna fecha se empalma

				return 0;

			}
			else if (validar_fecha2_CON_ID(FECHA_INGRESADA, HORA_INGRESADA, ID) == 4) { //No hay una diferencia de 1 hora al menos

				return 4;

			}
			else if (validar_fecha2_CON_ID(FECHA_INGRESADA, HORA_INGRESADA, ID) == 10) { //Todo salio correctamente

				return 10;

			}




		}

		int validar_fecha2_CON_ID(string FECHA_INGRESADA, string HORA_INGRESADA,int ID) {

			//Despues de validar lo basico en la primera funcion de validar fecha, pasamos a comparar las demas fechas de las otras citas

			if (origen != NULL && origen->next != NULL) { //Si ya tenemos mas de 2 estructuras

				string HORA1;
				string HORA2;

				string FECHA1;
				string FECHA2;

				aux = origen;
				FECHA1 = aux->Fecha;
				HORA1 = aux->Hora;

				FECHA2 = aux->next->Fecha;
				HORA2 = aux->next->Hora;

				int Posicion_FECHA1 = 0;

#pragma region ORDENAMOS_LAS_ESTRUCTURAS

				aux = origen;

				//Contamos cuantas estructuras tenemos
				int Cant_de_estructuras = 0;

				while (aux != NULL) {

					Cant_de_estructuras = Cant_de_estructuras + 1;

					aux = aux->next;

				}

				aux = origen;

				if (aux != NULL && aux->next != NULL) { //Si tenemos mas de una estrcurura
					//METODO BURBUJA


					for (int i = 0; i < Cant_de_estructuras; i++) {

						for (int j = 0; j < Cant_de_estructuras; j++) {

							if (aux->next != NULL && validar_fecha(aux->Fecha, aux->next->Fecha) == 2 && aux->ID != ID) { //Si estamos en al misma fecha || La ultima condicion es para saber si no tienen el msimo ID


									//Pasamos ahora a comparar las horas de dichas fechas, para corroborar que no se empalman
								HORA2 = aux->next->Hora;
								HORA1 = aux->Hora;

								if (validar_hora_FORMATO_12(HORA1, HORA2) == 0) {

									/*
									Mandamos a llamar la funcion con formato de 12 horas para converirlo a formato de 24 y comparar las horas
									Si son iguales las horas, se empalma la cita

									*/


									return 0; //Se empalma

								}
								else if (validar_hora_FORMATO_12(HORA1, HORA2) == 4) {

									return 4; //NO HAY UNA DIFERENCIA DE AL MENOS 1 HORA ENTRE CADA CITA

								}


							}

							aux = aux->next;
							/*
							if (aux->next != NULL) {
								aux = aux->next;
								aux = aux;

							}
							*/

						}

						aux = origen;


					}

				}

#pragma endregion

				return 10;

			}

			//En caso de ser apenas la segunda cita

			else if (origen != NULL && origen->next == NULL) { //Aqui vamos a comparar la hora ya registrada, con la hora

				string HORA1;
				string HORA2;

				string FECHA1;
				string FECHA2;

				//Igualamos las fechas que vamos a comparar para poderlas mandar como parametro
				FECHA2 = origen->Fecha;
				FECHA1 = FECHA_INGRESADA;


				//ESTO es para ver si la cita del origen no es igual a la cita de la 2da estructura


				if (validar_fecha(FECHA1, FECHA2) == 2 && origen->ID != ID) { //Cuando retorna 2 significa que estas citas estan en el mismo dia

					//Pasamos ahora a comparar las horas de dichas fechas, para corroborar que no se empalman
					HORA2 = origen->Hora;
					HORA1 = HORA_INGRESADA;

					if (validar_hora_FORMATO_12(HORA1, HORA2) == 0) {

						/*
						Mandamos a llamar la funcion con formato de 12 horas para converirlo a formato de 24 y comparar las horas
						Si son iguales las horas, se empalma la cita

						*/


						return 0; //Se empalma

					}
					else if (validar_hora_FORMATO_12(HORA1, HORA2) == 4) {

						return 4; //NO HAY UNA DIFERENCIA DE AL MENOS 1 HORA ENTRE CADA CITA

					}
					else if (validar_hora_FORMATO_12(HORA1, HORA2) == 10) {

						return 10; //Todo salio bien

					}


				}

				else { //Si ninguna fecha se topa, entonces no hay citas que se empalmen

					return 10;
				}
			}

			else if (origen == NULL) { //Si origen es igual NULL, no tenemos que comparar la fecha y hora que ninguna otra

				return 10; //Retornamos este numero, de que hemos terminado de validar

			}

		}

		int validar_fecha(string FECHA, string FECHA_ACTUAL) {

			//FECHA ACTUAL
			string str2_dia, str2_mes, str2_año;
			int dia_A, mes_A, año_A;

			//FECHA INGRESADA
			string str_dia, str_mes, str_año;
			int dia, mes, año;



			//FECHA INGRESADA//
			//DIA
			str_dia = FECHA.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
			dia = atoi(str_dia.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//MES
			str_mes = FECHA.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
			mes = atoi(str_mes.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//AÑO
			str_año = FECHA.substr(6);//Tomamos todos los caracteres a partir de la posicion 6
			año = atoi(str_año.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

					//FECHA ACTUAL//
					//DIA
			str2_dia = FECHA_ACTUAL.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
			dia_A = atoi(str2_dia.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//MES
			str2_mes = FECHA_ACTUAL.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
			mes_A = atoi(str2_mes.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//AÑO
			str2_año = FECHA_ACTUAL.substr(6);//Tomamos todos los caracteres a partir de la posicion 6
			año_A = atoi(str2_año.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"



					//COMPARAMOS


			if (año < año_A) {

				return 0;

			}
			else if (mes < mes_A && año == año_A) { //Si ingresa un mes menor al que estamos

				return 0;

			}
			else if (mes == mes_A && dia < dia_A) { //Esta fecha y paso

				return 0;

			}
			else if (mes == mes_A && dia == dia_A && año==año_A) { // SI ESTAMOS EN EL MISMO DIA

				return 2; //Retornara 2, si estamos en el mismo dia

			}

			

			else if (mes > mes_A && año == año_A || año > año_A) {

				return 5;

			}
			else if (mes == mes_A && dia > dia_A) {

				return 5;

			}



		}



		int validar_hora(string Hora, string Hora_ACTUAL) {

			char pm[6] = "p. m.";
			char am[6] = "a. m.";


			//HORA INGRESADA
			string str_hora, str_minutos, str_segundos, str_AMoPM;
			int hora, minutos, segundos;

			//HORA ACTUAL
			string str2_hora, str2_minutos, str2_segundos;
			int hora_A, minutos_A, segundos_A;



			//HORA INGRESADA//
			//HORA
			str_hora = Hora.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
			hora = atoi(str_hora.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//MINUTOS
			str_minutos = Hora.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
			minutos = atoi(str_minutos.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			// AM O PM???
			str_AMoPM = Hora.substr(9, 12);

			//Aqui haremos la hora en formato de 24 horas
			if (strcmp(str_AMoPM.c_str(), pm) == 0) {

		

					hora = hora + 12;
		
			}
			else if (strcmp(str_AMoPM.c_str(), am) == 0) {

				if (hora == 12) {

					hora = 0;

				}

			}

			//HORA ACTUAL//
			//HORA
			str2_hora = Hora_ACTUAL.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
			hora_A = atoi(str2_hora.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//MINUTOS
			str2_minutos = Hora_ACTUAL.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
			minutos_A = atoi(str2_minutos.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"


			if (hora == hora_A && minutos == minutos_A) { //Si se empalma la misma hora

				return 0;

			}
			else if (hora < hora_A) { //Esta hora de hoy ya paso

				return 2;

			}
			else if (hora == hora_A && minutos<minutos_A) { //Esta hora de hoy ya paso

				return 3;

			}




		}

		int validar_hora_FORMATO_12(string Hora_1, string Hora_2) {

			char pm[6] = "p. m.";
			char am[6] = "a. m.";


			//HORA INGRESADA
			string str_hora, str_minutos, str_segundos, str_AMoPM;
			int hora, minutos, segundos;

			//HORA ACTUAL
			string str2_hora, str2_minutos, str2_segundos, str2_AMoPM;
			int hora_A, minutos_A, segundos_A;

			//HORA INGRESADA//
			//HORA
			str_hora = Hora_1.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
			hora = atoi(str_hora.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			//MINUTOS
			str_minutos = Hora_1.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
			minutos = atoi(str_minutos.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

			// AM O PM???
			str_AMoPM = Hora_1.substr(9, 12);

			//Aqui haremos la hora en formato de 24 horas
			if (strcmp(str_AMoPM.c_str(), pm) == 0) {

					hora = hora + 12;
		
			}
			else if (strcmp(str_AMoPM.c_str(), am) == 0) {

				if (hora == 12) {

					hora = 0;

				}

			}


							//HORA 2//
							//HORA
							str2_hora = Hora_2.substr(0, 2); //Tomamos los caracteres de la posicion 0 a la 1
							hora_A = atoi(str2_hora.c_str()); //.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

							//MINUTOS
							str2_minutos = Hora_2.substr(3, 4);//Tomamos los caracteres de la posicion 3 a la 4
							minutos_A = atoi(str2_minutos.c_str());//.c_str extrae un puntero a un arreglo que contiene todos los "caracteres no nulos de la cadena"

							str2_AMoPM = Hora_1.substr(9, 12);

							//Aqui haremos la hora en formato de 24 horas
							if (strcmp(str2_AMoPM.c_str(), pm) == 0) {

									hora_A = hora_A + 12;
							}
							else if (strcmp(str2_AMoPM.c_str(), am) == 0) {

								if (hora == 12) {

									hora_A = 0;

								}

							}


			if (hora == hora_A && minutos == minutos_A) { //Si se empalma la misma hora

				return 0; //Si se empalma

			}
			else {

				int minutos_dia;
				int minutos_dia2;

				minutos_dia = (hora*60) + minutos;
				minutos_dia2 = (hora_A*60) + minutos_A;

				if (minutos_dia > minutos_dia2 && minutos_dia < minutos_dia2 + 60 || minutos_dia < minutos_dia2 && minutos_dia >minutos_dia2 - 60) {

					/*
					En este IF vemos si hay una diferencia de 1 hora entre cada cita, si no hay diferencia de al menos una hora retornamos 4
					*/

					return 4;
				}
				else {

					return 10;

				}

		

			}




		}



		
#pragma endregion