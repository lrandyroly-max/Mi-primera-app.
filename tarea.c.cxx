#include <stdio.h>
#include <string.h>

#define MAX_TAREAS 100
#define LONG_DESC 100

typedef struct
{
	int id;
	char description[LONG_DESC];
	int complete; // 0 = no, 1 = si
} Tarea;

Tarea tareas[MAX_TAREAS];
int numTareas = 0;
int proximoId = 1; /* para asignar ids correlativos */

void agragarTarea()
{
	if (numTareas >= MAX_TAREAS)
	{
		printf("No puedes añadir mas tareas.\nBorra alguna primero.");
		return;
	}
	printf("Descripcion de la tarea: ");
	/* Leer linea con posible espacio(usamos fgets) */
	getchar(); /* para limpiar el buffer del sañto de linea anterior*/
	fgets(tareas[numTareas].description, LONG_DESC, stdin);
	/*Quitar el salto de linea que fgets deja al final*/
	tareas[numTareas].description[strcspn(tareas[numTareas].description, "\n")] = 0;

	tareas[numTareas].id = proximoId++;
	tareas[numTareas].complete = 0;
	numTareas++;

	printf("Tarea añadida con ID %d \n", tareas[numTareas - 1].id);
}

void listarTarea()
{
	if (numTareas == 0)
	{
		printf("No hay tareas.\n");
		return;
	}
	printf("\n=== LISTA DE TAREAS ===\n");
	for (int i = 0; i < numTareas; i++)
	{
		printf("[%d]. %s -- %s\n", tareas[i].id, tareas[i].description, tareas[i].complete ? "Completada" : "Pendiente");
	}
	printf("====================\n");
}

void guardarTareas()
{
	FILE *archivo = fopen("tareas.dat", "wb");
	if (archivo == NULL)
	{
		printf("!!Error al guardar las tareas.!!\n");
		return;
	}
	/*Guardamos el numero de tareas y el proximo id*/
	fwrite(&numTareas, sizeof(int), 1, archivo);
	fwrite(&proximoId, sizeof(int), 1, archivo);
	fwrite(tareas, sizeof(Tarea), numTareas, archivo);
	fclose(archivo);
	printf("Tareas guardadas!!\n");
}

void cargarTareas()
{
	FILE *archivo = fopen("tareas.dat", "rb");
	if (archivo == NULL)
	{
		printf("No hay archivo previo. Empezando nuevo listado.\n");
		return;
	}
	/*Cargamos las tares existentes*/
	fread(&numTareas, sizeof(int), 1, archivo);
	fread(&proximoId, sizeof(int), 1, archivo);
	fread(tareas, sizeof(Tarea), numTareas, archivo);

	fclose(archivo);
	printf("Tareas cargadas desde archivo.\n");
}

void borrarTareas()
{
	if (numTareas == 0)
	{
		printf("No hay tareas para borrar.\n");
		return;
	}
	int id;
	printf("Introduce el ID de la tarea a borrar: ");
	scanf("%d", &id);

	int indice = -1;
	for (int i = 0; i < numTareas; i++)
	{
		if (tareas[i].id == id)
		{
			indice = i;
			break;
		}
		if (indice = -1)
		{
			printf("No existe tarea con ID %d", id);
			return;
		}
	}

	for (int i = indice; i < numTareas - 1; i++)
	{
		tareas[i] = tareas[i + 1];
	}
	numTareas--;
	printf("Tarea borrada.\n");
}

int main(int argc, char *argv[])
{
	int option;

	cargarTareas();

	do
	{
		printf("==GESTOR DE TAREAS==\n");
		printf("1. Añadir tarea.\n");
		printf("2. Listar tarea.\n");
		printf("3. Borrar tarea.\n");
		printf("4. Salir.\n");
		printf("Elige una opcion: \n");
		scanf("%d", &option);

		switch (option)
		{
		case 1:
			agragarTarea();
			break;
		case 2:
			listarTarea();
			break;
		case 3:
			borrarTareas();
			break;
		case 4:
			printf(" Ha sido un placer!!!!\n");
			break;
		default:
			printf(" Opcion incorrecta.\n");
		}

	} while (option != 4);

	guardarTareas();

	return 0;
}