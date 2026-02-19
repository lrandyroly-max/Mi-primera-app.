/*___
__Mi-primera-app___version-mejorada.
__Gestor de tareas con persitencia en archivos.
__Funcionalidades:agregar, listar, marcar completadas, borrar completadas.
*/

#include <stdio.h>
#include <string.h>

/*Macros para limites*/
#define MAX_TAREAS 100
#define MAX_DESCR 256
#define ARCHIVO "tareas.txt"

/*Estructura de una tarea*/
typedef struct
{
	int id;
	char description[MAX_DESCR];
	int completada; //0 = no, 1 = si
} Tarea;

/*Prototipos de funciones a usar*/
void agregarTarea();
void listarTareas();
void marcarCompletada();
void borrarCompletadas();
int generarNuevoId();

/*Funciones auxiliares para manejo de archivos*/
int cargarTareas(Tarea tareas[]);
void guardarTareas(Tarea tareas[], int total);

int main(int argc, char *argv[])
{
	int opcion;
	do
	{
		printf("===Gestor de Tareas===\n");
		printf("  1.Agregar tarea.\n");
		printf("  2.Listar tareas.\n");
		printf("  3.Marcar completada.\n");
		printf("  4.Borrar completadas.\n");
		printf("  5.Salir.\n\n");
		printf("Selecione una opcion: \n");
		scanf("%d", &opcion);

		while (getchar() != '\n')
			;

		switch (opcion)
		{
		case 1:
			agregarTarea();
			break;
		case 2:
			listarTareas();
			break;
		case 3:
			marcarCompletada();
			break;
		case 4:
			borrarCompletadas();
			break;
		case 5:
			printf("===EXIT!!!!===\n");
			break;
		default:
			printf("OPCION INVALIDA...\n");
		}

	} while (opcion != 5);

	return 0;
}

void agregarTarea()
{
	FILE *archivo = fopen(ARCHIVO, "a");
	if (archivo == NULL)
	{
		printf("Error al abrir el archivo!!!\n");
		return;
	}

	Tarea nueva;
	nueva.id = generarNuevoId();
	nueva.completada = 0; /*pendiente por default.*/
	printf("Introduce la descripcion de la tarea.\n");
	fgets(nueva.description, MAX_DESCR, stdin);
	/*eliminar el salto de linea que provoca fgets.*/
	size_t len = strlen(nueva.description);
	if (len > 0 && nueva.description[len - 1] == '\n')
	{
		nueva.description[len - 1] = '\0';
	}

	fprintf(archivo, "%d %d %s\n", nueva.id, nueva.completada, nueva.description);
	fclose(archivo);

	printf("Tarea agregada con ID %d.\n", nueva.id);
}

void listarTareas()
{
	FILE *archivo = fopen(ARCHIVO, "r");
	if (archivo == NULL)
	{
		printf("No hay tareas guardadas.(archivo inexistente).\n");
		return;
	}

	printf("\n===LISTA DE TAREAS===\n");
	char linea[MAX_DESCR + 20];
	int encontradas = 0;

	while (fgets(linea, sizeof(linea), archivo))
	{
		if (linea[0] == '\n' || linea[0] == '\0')
		{
			continue;
		}

		int id, completada;
		char desc[MAX_DESCR];
		if (sscanf(linea, "%d %d %[^\n]", &id, &completada, desc) == 3)
		{
			printf("ID: %d | %s | %s\n",
				   id,
				   completada ? "COMPLETADA" : "PENDIENTE", desc);
			encontradas++;
		}
		else
		{
			printf("Linea con formato invalido: %s", linea);
		}
	}

	fclose(archivo);

	if (encontradas == 0)
		printf("No hay tareas registradas.\n");
	printf("\n====================\n");
}

void marcarCompletada()
{
	Tarea tareas[MAX_DESCR];
	int total = cargarTareas(tareas);

	if (total == 0)
	{
		printf("No existen tareas para listar.\n");
		return;
	}

	int id;
	printf("Ingrese el ID de la tarea que desea marcar.\n");
	scanf("%d", &id);

	int encontrada = 0;
	for (int i = 0; i < total; i++)
	{
		if (tareas[i].id == id)
		{
			if (tareas[i].completada == 1)
			{
				printf("La tarea ya estaba completada.\n");
			}
			else
			{
				tareas[i].completada = 1;
				printf("Tarea ID %d marcada como COMPLETADA.\n", id);
			}

			encontrada = 1;
			break;
		}
	}

	if (!encontrada)
	{
		printf("No existe una tarea con ID %d.\n", id);
		return;
	}

	guardarTareas(tareas, total);
}

void borrarCompletadas()
{
	Tarea tareas[MAX_DESCR];
	int total = cargarTareas(tareas);

	if (total == 0)
	{
		printf("No hay tareas que borrar.\n");
		return;
	}

	int option;
	printf("Que desea borrar?\n");
	printf("1. Solo tareas completadas.\n");
	printf("2. Todas las tareas.\n");
	printf("Option:  ");
	scanf("%d", &option);

	if (option == 2)
	{
		if (remove(ARCHIVO) == 0)
		{
			printf("Todas las tareas han sido eliminadas.\n");
		}
		else
		{
			printf("ERROR al borrar el archivo.\n");
		}
		return;
	}
	else if (option == 1)
	{
		//Filtrar conservar solo las pendientes
		Tarea nuevas[MAX_TAREAS];
		int nuevas_count = 0;
		for (int i = 0; i < total; i++)
		{
			if (tareas[i].completada == 0)
			{
				nuevas[nuevas_count++] = tareas[i];
			}
		}

		if (nuevas_count == total)
		{
			printf("No hay tareas completadas para borrar.\n");
			return;
		}

		guardarTareas(nuevas, nuevas_count);
		printf("Se borraron %d tarea(s) completada(s), quedan %d tarea(s) pendiente(s).\n", total - nuevas_count, nuevas_count);
	}
	else{
		printf("Opcion no valida.\n");
		}
}

int generarNuevoId()
{
	FILE *archivo = fopen(ARCHIVO, "r");

	if (archivo == NULL)
	{
		//el archivo no existe, es la primera tarea
		return 1;
	}

	int maxId = 0;
	char linea[256];
	while (fgets(linea, sizeof(linea), archivo))
	{
		if (linea[0] == '\n' || linea[0] == '\0')
			continue;
		int id, completada;
		char desc[256];
		if (sscanf(linea, "%d %d %[^\n]", &id, &completada, desc) == 3)
		{
			if (id > maxId)
				maxId = id;
		}
	}
	fclose(archivo);
	return maxId + 1;
}

int cargarTareas(Tarea tareas[])
{
	FILE *archivo = fopen(ARCHIVO, "r");
	if (archivo == NULL)
	{
		return 0;
	}

	int count = 0;
	char linea[MAX_DESCR + 20];
	while (count < MAX_TAREAS && fgets(linea, sizeof(linea), archivo))
	{
		if (linea[0] == '\n' || linea[0] == '\0')
		{
			continue;
		}

		Tarea t;
		if (sscanf(linea, "%d %d %[^\n]", &t.id, &t.completada, t.description) == 3)
		{
			tareas[count++] = t;
		}
	}
	fclose(archivo);
	return count;
}

void guardarTareas(Tarea tareas[], int total)
{
	FILE *archivo = fopen(ARCHIVO, "w");
	if (archivo == NULL)
	{
		printf("Error al guardar las tareas.\n");
		return;
	}

	for (int i = 0; i < total; i++)
	{
		fprintf(archivo, "%d %d %s\n", tareas[i].id, tareas[i].completada, tareas[i].description);
	}

	fclose(archivo);
}