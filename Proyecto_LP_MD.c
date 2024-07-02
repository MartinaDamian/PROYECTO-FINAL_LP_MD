/*
        PROYECTO FINAL - PROGRAMACION I - PROGRESO 3
        INTEGRANTES: MARTINA DAMIAN Y LUIS POZO

        NOMBRE DEL PROGRAMA: LISTADO_AMAZON

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
const char* direccion="C:\\Users\\Usuario\\Desktop\\Proyecto_final\\";//direccion donde se encontraran los archivos, se declara como constante
struct d_pro{// Estructura de producto
    char name[20];//Nombre del producto
    char stock[4];// Cantidad del producto, es char, debido a que no se hara ningun calculo con esta cantidad, con el fin de ahorrar memoria
    float ps_i; // El usuario ingresara el precio del producto sin iva
    float iva; //Calculo del iva
    float pc_i; // Precio con iva
    }producto;
char* ARCHIVO(){
    char* nom_arch=NULL; //utilizamos un puntero para almacenar la ruta del archivo
    char nom[20]; // arreglo para el nombre del archivo
    printf("\nARCHIVO: ");
    scanf("%19s", nom);//pide el ingreso del nombre del archivo, este tiene un maximo de 19 caracteres
    nom_arch= (char*)malloc(strlen(direccion)+strlen(nom)+1);// Asigna la memoria para la ruta del archivo
    strcpy(nom_arch, direccion);//copia la direccion
    strcat(nom_arch, nom);// strcat es para concatenar la direccion y el nombre
    return nom_arch;//nom_arch tiene la direccion completa del archivo junto al nombre del archivo
}
int LEER(char* n_arch){//Funcion de vario uso para la impresion de los archivos en forma de lista
    int np=0;// Contador de cuantos productos se encuentra en el archivo
    FILE*fil=fopen(n_arch, "rb");// Se abre el archivo unicamente para lectura en este caso de un archivo binario
    if(fil==NULL){
        printf("ERROR AL ENCONTRAR EL ARCHIVO"); //Si el archivo es nulo te dice error al encontrar el archivo
        return 0;}
    printf(" No.|     NOMBRE        |   CANTIDAD  |       PRECIO     |      IVA      |   PRECIO FINAL\n");
    while(fread(&producto, sizeof(producto), 1, fil)==1){ //en este while se lee el archivo mientras hayan datos
    if(strcmp(producto.name,"DELETED")!=0){// Esta if no se realizara tras realizar alguna eliminacion de la funcion ELIMINAR
    np++; //incrementa contador de #de productos
    printf("  %d.\t%-20s %-3s\t\t%.2f\t\t%.2f\t\t%.2f\n",np, producto.name, producto.stock, producto.ps_i, producto.iva, producto.pc_i);}} //imprime los detalles del producto
    fclose(fil); //cierra el documento
    return np; //retorna el valor del numero de productos
}
void ANADIR(char* n_arch){//Funcion para ingresar productos, tambien sirve para la funcion de crear
    int ans3;
    FILE* fil;
    // Abrir el archivo en modo de lectura para verificar su existencia
    fil=fopen(n_arch, "rb");
    if(fil==NULL){
        perror("ERROR: ARCHIVO NO ENCONTRADO"); //misma validacion para abrir el archivo
        return;}
    fclose(fil);

    fil=fopen(n_arch, "ab"); //abre documento para añadir mas informacion
    if(fil==NULL){
        perror("ERROR AL ABRIR EL ARCHIVO"); //misma validacion para abrir el archivo
        return;}
    do{//Ingreso de productos usando estructura producto con un bucle
        fflush(stdin);
        printf("Nombre del producto: ");
        scanf("%19[^\n]", producto.name);
        printf("Cantidad del producto: ");
        scanf("%3s", producto.stock);
        printf("Precio del producto sin IVA: ");
        scanf("%f", &producto.ps_i);
        producto.iva=producto.ps_i*0.15;
        producto.pc_i=producto.ps_i+producto.iva;
        fwrite(&producto, sizeof(producto), 1, fil);
        printf("Desea ingresar a la lista otro producto? (1)Si (2)No\n");
        scanf("%d", &ans3);
    }while(ans3!=2);//Bucle para seguir anadiendo productos
    fclose(fil);
    printf("INGRESO DE PRODUCTO\\S COMPLETADO\n");
    LEER(n_arch);
}
void CREAR(char* nom_arch){
    int ans;
    FILE* fil;
    if((fil=fopen(nom_arch,"rb"))==NULL){//Estructura anidada, con el fin de evitar errores al eliminar el contenido de una lista ya existente sin previo aviso
        if((fil=fopen(nom_arch, "wb"))!=NULL){
        system("cls"); // esto se utiliza para limpiar la pantalla de la consola
        ENCABEZADO(); //funcion para imprimir información de la lista con amazon
        printf("LISTA CREADA CON EXITO\n");
        fclose(fil);
        printf("INGRESO DE PRODUCTOS:\n");
        ANADIR(nom_arch);} //se añaden los productos al documento creado
        else{
        printf("ERROR AL CREAR EL ARCHIVO\n"); //si no se abre el archivo para escribir en modo binario, retorna esto
        return;}}
    else{
        printf("YA EXISTE ESA LISTA\nDesea remplazar lista ya creada anteriormente? (1)Si (2)No: ");
        scanf("%d", &ans);
        if (ans==1){
            if((fil=fopen(nom_arch, "wb"))!=NULL){
            system("cls"); // esto se utiliza para limpiar la pantalla de la consola
            ENCABEZADO(); //funcion para imprimir información de la lista con amazon
            printf("LISTA CREADA CON EXITO\n");
            fclose(fil);
            printf("INGRESO DE PRODUCTOS:\n");
            ANADIR(nom_arch);} //se añaden los productos al documento creado
        }
        fclose(fil); // si pones 2 se cierra el documento
        return;}
    free(nom_arch); //para liberar la memoria utilizada para nom_arch

}
void EDITAR(char* nom_arch, int num_pro){ //utilizamos el numero del producto y el nombre del archivo para saber que editar
    int pro_edit, edit_ans, ans;
    FILE* fil=fopen(nom_arch, "rb+");
    if(fil==NULL){
        printf("\nERROR AL ABRIR EL ARCHIVO");
        return;}
    fread(&producto, sizeof(producto), num_pro, fil); //leer numero de productos
    edit: //creacion de label
    printf("Ingresar el No. del producto que desea editar:\n");
    printf("Producto: ");
    scanf("%d", &pro_edit);
    if (pro_edit<1||pro_edit>num_pro){ //if para poder asegurar que se escoja un producto valido
        printf("\n***Ingresar un numero de producto valido***\n");
        goto edit;} //manda al label de arriba
    pro_edit--;//como los arreglos comienzan con 0, le bajamos a la variable para que sea igual que la info en la matriz
    fseek(fil, pro_edit*sizeof(struct d_pro), SEEK_SET);//mueve el puntero a una ubicacion en especifico
    fread(&producto, sizeof(struct d_pro), 1, fil); //lee los datos del documento y dentro de producto
    printf("El producto a editar es:\n");
    printf("1. Nombre: %s\n2. Cantidad: %s\n3. Precio: %.2f\n\n***El IVA y el precio total no son modificables, van de acuerdo al precio sin IVA ingresado***\n", producto.name, producto.stock, producto.ps_i, producto.iva, producto.pc_i);
    carac_edit: //creacion de label
    printf("ESCOGER LA CARACTERISTICA A EDITAR: ");
    scanf("%d", &edit_ans);
    if (edit_ans<1 || edit_ans>3){ //valida el numero de caracteristicas
        printf("NUMERO NO VALIDO");
        goto carac_edit;}
    fflush(stdin); //vacía el buffer
    switch(edit_ans){ //creacion de funcion switch para facilitar edicion
        case 1:
            printf("Nuevo nombre del producto: ");
            scanf("%19[^\n]", producto.name);
            break;
        case 2:
            printf("Nueva cantidad del producto: ");
            scanf("%3s", producto.stock);
            break;
        case 3:
            printf("Precio del producto sin IVA: ");
            scanf("%f", &producto.ps_i);
            producto.iva=producto.ps_i*0.15;
            producto.pc_i=producto.ps_i+producto.iva;
            break;
    }
    printf("Desea editar otra caracteristica del mismo producto? (1)Si (2)No:  ");
    scanf("%d", &ans);
    if (ans==1){goto carac_edit;} //te manda al label de arriba
    fseek(fil, pro_edit*sizeof(struct d_pro), SEEK_SET);//posicion del documento es mandada otra vez a la ubicacion del producto
    fwrite(&producto, sizeof(struct d_pro), 1, fil); //se modifica el producto
    fclose(fil);
    system("cls");
    ENCABEZADO();
    printf("\nPRODUCTO MODIFICADO CON EXITO\n");
    LEER(nom_arch);
    }
void ELIMINAR(char* nom_arch, int num_pro){ //Funcion toma dos elementos
    int pro_edit, edit_ans, ans;
    FILE* fil=fopen(nom_arch, "rb+");
    if(fil==NULL){
        printf("\nERROR AL ABRIR EL ARCHIVO");
        return;}
    fread(&producto, sizeof(producto), num_pro, fil);
    edit:
    printf("Ingresar el No. del producto que desea eliminar:\n");
    printf("Producto: ");
    fflush(stdin);
    scanf("%d", &pro_edit);//Usuario ingresa numero de producto
    if (pro_edit<1||pro_edit>num_pro){
        printf("\n***Ingresar un numero de producto valido***\n");
        goto edit;}
    pro_edit--; //para que inicie igual que la matriz
    fseek(fil, pro_edit*sizeof(struct d_pro), SEEK_SET);
    fread(&producto, sizeof(struct d_pro), 1, fil);
    printf("El producto a eliminar es:\n");
    printf("Nombre: %s\nCantidad: %s\nPrecio: %.2f\n\nIVA: %.2f\nPrecio con IVA: %.2f\n", producto.name, producto.stock, producto.ps_i, producto.iva, producto.pc_i);
    printf("Esta segur@ de eliminar el producto? (1)Si (2)No:  ");
    fflush(stdin);
    scanf("%d", &ans);
    if (ans==2){
        printf("PROCESO DE ELIMINACION CANCELADO\n");
        return;}
    fseek(fil, pro_edit*sizeof(struct d_pro), SEEK_SET); //posición es devuelta a la ubicacion del producto
    struct d_pro producto={"DELETED","",0.0,0.0,0.0}; //Una estructura de datos con el nombre "DELETED" para marcar al producto como eliminado
    fwrite(&producto, sizeof(struct d_pro), 1, fil);
    fclose(fil);
    system("cls");
    ENCABEZADO(); //funcion de encabezado para mostrar los datos con amazon
    printf("\nPRODUCTO ELIMINADO CON EXITO\n");
    LEER(nom_arch);
    printf("Deseas eliminar otro producto de esta lista? (1)Si (2)No\n");
    scanf("%d", &ans);
    if(ans==1){goto edit;}// te devuelve al label
    }
void ENCABEZADO(){ //funcion para un encabezado lindo!
    printf("******************************************************************************************\n");
    printf("                               A   M   A   Z   O   N                                      \n");
    printf("******************************************************************************************\n");
}
int menu(){ //funcion en valor entero para procesar la decision del usuario
    int ans1, nump;
    ENCABEZADO();
    do{
    printf("\nMENU\n\n1. Visualizar lista existente\n2. Crear lista de stock\n3. Anadir stock\n4. Editar detalles de producto\n5. Eliminar stock\n6. Salir del sistema\n Escoger:\t");fflush(stdin);
    scanf("%d", &ans1);
    }while(ans1<1 || ans1>6);
    switch (ans1){ //switch para poder tener todas las opciones disponibles
    case 1:{
        //1. Visualizar lista existente
        system("cls");
        ENCABEZADO();
        printf("LECTURA DE LISTA\n\nIngresar el nombre de la lista:\n");
        char* nom1_arch=ARCHIVO();
        LEER(nom1_arch);
        free(nom1_arch);
        break;}
    case 2:{// 1. Crear lista de stock
        system("cls");
        ENCABEZADO();
        printf("CREACION DE LISTA\n\nIngresar el nombre de la lista a crear:\n");
        char* nom_arch=ARCHIVO();
        CREAR(nom_arch);
        break;}
    case 3:{//3. Anadir stock
        system("cls");
        ENCABEZADO();
        printf("INGRESO DE NUEVOS PRODUCTOS\nIngresar la lista donde se agregaran productos:\n");
        char* nom2_arch=ARCHIVO();
        ANADIR(nom2_arch);
        free(nom2_arch);
        break;}
    case 4:{//Editar detalles de producto
        system("cls");
        ENCABEZADO();
        printf("EDICION DE PRODUCTOS\n");
        char* nom3_arch= ARCHIVO();
        nump=LEER(nom3_arch);
        EDITAR(nom3_arch, nump);
        free(nom3_arch);
        break;}
    case 5:{
        //4. Eliminar stock
        system("cls");
        ENCABEZADO();
        printf("ELIMINACION DE PRODUCTOS\n");
        char* nom4_arch= ARCHIVO();
        nump=LEER(nom4_arch);
        ELIMINAR(nom4_arch, nump);
        free(nom4_arch);
         break;}
    case 6:{
        //5. Salir
         printf("GRACIAS POR USAR NUESTRO PROGRAMA");
        exit(0);}}
}
int main(){
    int ans;
    printf("\n\nBienvenid@ VV, se encuentra dentro del sistema de menu de la tienda AMAZON\n");
    menu1: //label para menu
    menu(); //funcion de menu
    do{
    printf("¿Deseas volver al menu? (1)Si (2)No:\t");fflush(stdin);
    scanf("%d", &ans);
    }while(ans!=1 && ans!=2);
    if(ans==1){
        system("cls");
        goto menu1;} //te manda al label para leer funcion del menu
    return 0;}

