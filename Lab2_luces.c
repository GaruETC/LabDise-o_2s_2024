#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//FUNCIONES
//Función que recibe el arreglo luces y el arreglo del boton que sera apretado(uno de los arreglos de la matriz)
void PresionarBoton(char *luces, int *boton){
	int num_luces = boton[0];//el primer valor del arreglo es la cantidad de luces que controla el boton
	for(int i=0;i<num_luces;i++){
		int pos_luz = boton[i+1]-1;//se resta 1 al numero de la luz porque es su posicion en el arreglo de luces(si la luz es 1, entonces estara en la posicion 0 del arreglo)
		if (luces[pos_luz] == 'R'){
			luces[pos_luz] = 'G';//si es roja cambia a verde
		}else if (luces[pos_luz] == 'G'){
			luces[pos_luz] = 'B';//si es verde cambia a azul
		}else if (luces[pos_luz] == 'B'){
			luces[pos_luz] = 'R';//si es azul cambia a roja
		}
	}
}

//Función que recibe el arreglo luces y el numero de luces y verifica si todas las luces son rojas
bool TodosRojos(char *luces, int n_luces){
	for(int i=0;i<n_luces;i++){
		if (luces[i] != 'R') {
			return false;//si alguna luz no es roja retorna false
		}
	}
	return true;//si todas son rojas retorna true
}

//función que recibe el arreglo de los botones presionados, el numero de botones y un numero que sera el que buscara cuantas veces está en el arreglo
int cant_toques(int *toques, int n_botones, int num){
	int total = 0;//inicializamos el total en 0
	for(int i=0;i<2*n_botones;i++){
		if (toques[i] == num){
			total += 1;//busco en cada posición del arreglo si es el numero y si lo es aumento en 1 el total
		}
	}
	return total;//retorno el total de veces que está el número en el arreglo
}

//Función que recibe el arreglo de los botones presionados, el numero de botones y un número que se agregara al arreglo toques
void AgregarAArreglo(int *toques, int n_botones, int num){
	for(int i=0;i<2*n_botones;i++){
		if (toques[i] == 0){
			toques[i] = num;//cuando encuentre un 0 significara que esa posicion está vacia y colocara el número
			return;//retorna justo despues de colocar el número
		}
	}
}

//Función que recibe el arreglo de los botones presionados y el numero de botones
void EliminarUltimo(int *toques, int n_botones){
	for(int i=2*n_botones-1;i>=0;i--){
		if (toques[i] != 0){
			toques[i] = 0;//va de derecha a izquierda hasta encontrar un número diferente de 0 y lo elimina
			return;//al eliminar el número termina la función
		}
	}
}

//Función que recibe el arreglo de luces(luces), la matriz con los botones y sus conexiones(matriz), la cantidad de botones y luces(n_botones,n_luces),
//la cantidad de veces que se han apretado botones(cantidad), el arreglo de los botones presionados(toques),
//un numero que le da al i del primer for un comienzo para que no repita algunas combinaciones(inicio)
//y el menor numero de veces que se apretan botones para llegar a que todas las luces son rojas(menor)
int Backtracking(char *luces, int **matriz, int n_botones, int n_luces, int cantidad, int *toques, int inicio, int menor){
	if (TodosRojos(luces, n_luces) && cantidad<menor){
		return cantidad;//si todas las luces son rojas y la cantidad es menor que "menor", retorna la cantidad de veces que se apretaron botones(la primera vez que TodosRojos es true, siemrpe retorna)
	}
	if(cantidad>=2*n_botones || cantidad>menor){
		return 2*n_botones+1;//si la cantidad llega al limite o es mayor que "menor" retorna
	}
	for(int i=inicio;i<n_botones;i++){
		inicio=i;
		if(cant_toques(toques, n_botones, i+1)<2){
			char *luces_copia = (char*)malloc(n_luces*sizeof(char));
			for(int j=0;j<n_luces;j++){
				luces_copia[j] = luces[j];//crea un arreglo copia de luces para no modificar el original
			}
			AgregarAArreglo(toques, n_botones, i+1);//agrega el botón que se tocá al arreglo toques
			PresionarBoton(luces_copia, matriz[i]);//cambia el color de las luces que están conectadas al botón
			int resultado = Backtracking(luces_copia, matriz, n_botones, n_luces, cantidad+1, toques, inicio, menor);
			EliminarUltimo(toques, n_botones);//elimina el botón que agrego anteriormente para seguir con otras combinaciones
			if (resultado<menor){
				menor = resultado;
			}

			free(luces_copia);
		}
	}
	return menor;
}

//Función main
int main(){
	int n_luces, n_botones;
	scanf("%d %d", &n_luces, &n_botones); //se asignan las cantidades de luces y botones a variables
	char *rgb = (char *)malloc((n_luces+1)*sizeof(char)); //se crea un arreglo para guardar el string de las luces
	scanf("%s", rgb); //se asigna el string al arreglo 

	int num, luz;
	int **matriz = (int**)malloc(n_botones*sizeof(int *));//se crea un arreglo de enteros para luego crear una "matriz"
	for(int i=0;i<n_botones;i++){
		scanf("%d", &num);//se asigna el numero de luces de cada boton a una variable
		matriz[i] = (int *)malloc((num+1)*sizeof(int));//se usa la variable anterior para crear cada arreglo que tendrá las el numero de luces en primera posicion, y luego las luces
		matriz[i][0] = num;//se coloca el numero de luces del boton
		for(int j=0;j<num;j++){
			scanf("%d", &luz);//se asigna la luz a una variable
			matriz[i][j+1] = luz;//se coloca esa luz en el arreglo de su boton
		}
	}

	int *toques = (int *)malloc((2*n_botones)*sizeof(int));//crea el arreglo donde estarán los botones presionados
	for(int i=0;i<2*n_botones;i++){
		toques[i] = 0;//llena el arreglo de 0
	}

	int menor=2*n_botones+1;//inicializa menor como un número al que nunca se llegará
	int final = Backtracking(rgb, matriz, n_botones, n_luces, 0, toques, 0, menor);

	if (final == 2*n_botones+1){
		printf("imposible");// si final es igual al numero con el que se inicializó significa que nunca encontró una solución
	}else{
		printf("%d", final);//si no es igual significa que si encontro una solución
	}

	for(int i=0;i< n_botones;i++) {
        free(matriz[i]);// Liberar cada fila
    }
    free(matriz);//libera el arreglo de punteros
	free(rgb);//libera el arreglo de luces
}