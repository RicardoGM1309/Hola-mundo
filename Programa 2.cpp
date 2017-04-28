//Instituto Tecnológico  Superior José Mario Molina Pasquel Y Henríquez Zapopan
//
//           Fecha:  04 de abril del 2017
//           Desarrollador:   Ricardo Gonzalez Martinez
//           Materia: Personal Software Process
//           Función del Programa:  Programa que contara las líneas de
//                                  Código de los archivo encontrados en una carpeta
//                                  requerida por el usuario.

//
//  El programa recibirá una cadena la cual será el nombre del archivo que el
//  usuario desea contar las líneas de código, este no debe de contener
//  caracteres especiales y deberá ser escrito igual que el nombre del archivo.
//  El programa no valida si los datos introducidos son de otro tipo del
//  Especificado, por lo que se deberá estar consciente de ello.
//

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <fstream>

using namespace std;

int Contador_de_lineas = 0, Contador_de_funciones = 0, Contador_de_metodos = 0, Contador_de_espacios = 0;
int Contador_de_clases = 0, Contador_de_archivos = 0, Contador_de_comentarios = 0;

//Funcion que retorna un error
void error(const char *s){
    perror (s);
    exit(EXIT_FAILURE);
}

//Funcion que procesa el archivo
void Procesar_Archivo(char *archivo, const char * c){
    FILE *fich;
    char caracter, Primer_Caracter, Segundo_caracter;
    int iterador = 0;

    char * carpeta = strdup(c);
    char * diagonal = "/";
    char *concatenacion = (char*) malloc(1 + strlen(carpeta) + strlen(diagonal));
    strcpy(concatenacion,carpeta);
    strcat(concatenacion,diagonal);

    char *Ruta_defecto = concatenacion;
    char *Ubicacion_archivo = (char*) malloc(1 + strlen(Ruta_defecto) + strlen(archivo));
    strcpy(Ubicacion_archivo,Ruta_defecto);
    strcat(Ubicacion_archivo,archivo);

    fich = fopen(Ubicacion_archivo, "r");
    if (fich == NULL){
        cout<<endl<<"Error en la apertura del archivo"<<endl<<endl;
    }else{
        string Tipo_item = "";
        fseek(fich, 0L, SEEK_SET);
        while(!feof(fich)){

            caracter = fgetc(fich);
            if(caracter == '/' && caracter == Primer_Caracter)
                Contador_de_comentarios++;
            if(caracter == '\n' && caracter == Primer_Caracter)
                Contador_de_espacios++;
            Primer_Caracter = caracter;

            if(caracter == '\n')
                Contador_de_lineas++;
            if(!isspace(caracter) || caracter != '\n'){
                Tipo_item = Tipo_item + caracter;
                if(Tipo_item == "//Funcion"){
                    Contador_de_funciones++;
                    Contador_de_comentarios--;
                }
                else if(Tipo_item == "//Metodo"){
                    Contador_de_comentarios--;
                    Contador_de_metodos++;
                }
                else if(Tipo_item == "//Clase"){
                    Contador_de_comentarios--;
                    Contador_de_clases++;
                }
            }else if(isspace(caracter) || caracter == '\n')
                Tipo_item = "";
        };
        fclose(fich);
    }
}

//Funcion que abre el directorio
int Abrir_directorio(string carpeta){
    DIR *dir;
    struct dirent *ent;

    const char *c  = carpeta.c_str();
    dir = opendir (c);
    if (dir == NULL)
        error("No puedo abrir el directorio");
        while ((ent = readdir (dir)) != NULL){
            if((strcmp(ent->d_name,".")!=0)&&(strcmp(ent->d_name,"..")!=0)){
                cout<<"     "<<ent->d_name<<endl;
                Contador_de_archivos++;
                Procesar_Archivo(ent->d_name, c);
            }
        }
    closedir (dir);
    return EXIT_SUCCESS;
}

//Funcion principal
int main(){
    string Nombre_directorio = "";
    cout<<"Ingresa el nombre de la carpeta a analizar: ";
    cin>>Nombre_directorio;

    cout<<endl<<endl<<"Archivos analizados: "<<endl;
    Abrir_directorio(Nombre_directorio);

    cout<<endl<<endl<<"Total de funciones: "<<Contador_de_funciones<<endl;
    cout<<"Total de clases: "<<Contador_de_clases<<endl;
    cout<<"Total de metodos: "<<Contador_de_metodos<<endl;
    cout<<"Total de lineas: "<<Contador_de_lineas + (Contador_de_archivos - (Contador_de_espacios + (Contador_de_comentarios)))<<endl;

return 0;
}
