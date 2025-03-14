#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_PACIENTES 20
#define MAX_SERVICIOS 6
#define ARCHIVO_PACIENTES "pacientes.dat"
#define ARCHIVO_HOSPITALIZADOS "hospitalizados.dat"
#define ARCHIVO_EMERGENCIAS "emergencias.dat"

typedef struct {
    char nombre[50];
    int edad;
    char cedula[20];
} Paciente;

typedef struct {
    char nombre[50];
    float precio_dolares;
} Servicio;

typedef struct {
    Paciente paciente;
    char causa[100];
    int dias_hospitalizado;
    int piso;
    int habitacion;
} Hospitalizacion;

typedef struct {
    Paciente paciente;
    char hora[10];
    char fecha[20];
} Emergencia;

Servicio servicios[MAX_SERVICIOS] = {
    {"Hematologia completa", 0},
    {"Perfil tiroideo", 0},
    {"Cortisol", 0},
    {"Glicemia", 0},
    {"Colesterol", 0},
    {"Trigliceridos", 0}
};

Paciente pacientes[MAX_PACIENTES];
Hospitalizacion hospitalizados[MAX_PACIENTES];
Emergencia emergencias[MAX_PACIENTES];
int total_pacientes = 0;
int total_hospitalizados = 0;
int total_emergencias = 0;

void generarPreciosServicios() {
    srand(time(NULL));
    for (int i = 0; i < MAX_SERVICIOS; i++) {
        servicios[i].precio_dolares = rand() % 1000 + 1;
    }
}

void cargarPacientesDesdeArchivo() {
    FILE *file = fopen(ARCHIVO_PACIENTES, "rb");
    if (file) {
        while (fread(&pacientes[total_pacientes], sizeof(Paciente), 1, file)) {
            total_pacientes++;
        }
        fclose(file);
    }
}

void guardarPacientesEnArchivo() {
    FILE *file = fopen(ARCHIVO_PACIENTES, "wb");
    if (file) {
        fwrite(pacientes, sizeof(Paciente), total_pacientes, file);
        fclose(file);
    }
}

void agregarPaciente() {
    if (total_pacientes < MAX_PACIENTES) {
        Paciente p;
        printf("Ingrese el nombre del paciente: ");
        scanf("%s", p.nombre);
        printf("Ingrese la edad del paciente: ");
        scanf("%d", &p.edad);
        printf("Ingrese la cedula del paciente: ");
        scanf("%s", p.cedula);
        pacientes[total_pacientes++] = p;
        guardarPacientesEnArchivo();
    } else {
        printf("No se pueden agregar mas pacientes.\n");
    }
}

void agregarHospitalizacion() {
    if (total_hospitalizados < MAX_PACIENTES) {
        Hospitalizacion h;
        printf("Ingrese la cedula del paciente: ");
        char cedula[20];
        scanf("%s", cedula);
        for (int i = 0; i < total_pacientes; i++) {
            if (strcmp(pacientes[i].cedula, cedula) == 0) {
                h.paciente = pacientes[i];
                printf("Ingrese la causa de la hospitalizacion: ");
                scanf("%s", h.causa);
                printf("Ingrese los dias hospitalizados: ");
                scanf("%d", &h.dias_hospitalizado);
                printf("Ingrese el piso: ");
                scanf("%d", &h.piso);
                printf("Ingrese el numero de habitacion: ");
                scanf("%d", &h.habitacion);
                hospitalizados[total_hospitalizados++] = h;
                FILE *file = fopen(ARCHIVO_HOSPITALIZADOS, "ab");
                if (file) {
                    fwrite(&h, sizeof(Hospitalizacion), 1, file);
                    fclose(file);
                }
                return;
            }
        }
        printf("Paciente no encontrado.\n");
    } else {
        printf("No se pueden agregar mas hospitalizaciones.\n");
    }
}

void agregarEmergencia() {
    if (total_emergencias < MAX_PACIENTES) {
        Emergencia e;
        printf("Ingrese la cedula del paciente: ");
        char cedula[20];
        scanf("%s", cedula);
        for (int i = 0; i < total_pacientes; i++) {
            if (strcmp(pacientes[i].cedula, cedula) == 0) {
                e.paciente = pacientes[i];
                printf("Ingrese la hora de ingreso (HH:MM): ");
                scanf("%s", e.hora);
                printf("Ingrese la fecha de ingreso (DD/MM/AAAA): ");
                scanf("%s", e.fecha);
                emergencias[total_emergencias++] = e;
                FILE *file = fopen(ARCHIVO_EMERGENCIAS, "ab");
                if (file) {
                    fwrite(&e, sizeof(Emergencia), 1, file);
                    fclose(file);
                }
                return;
            }
        }
        printf("Paciente no encontrado.\n");
    } else {
        printf("No se pueden agregar mas emergencias.\n");
    }
}

void calcularMontoTotal(Paciente paciente) {
    float total_dolares = 0.0;

    printf("Servicios prestados a paciente %s:\n", paciente.cedula);
    for (int i = 0; i < MAX_SERVICIOS; i++) {
        total_dolares += servicios[i].precio_dolares;
        printf("Servicio: %s - Precio: $%.2f\n", servicios[i].nombre, servicios[i].precio_dolares);
    }

    float total_bolivares = total_dolares * 4; // Suponiendo un tipo de cambio de 1$ = 4 Bs
    printf("Monto total en Dolares: $%.2f\n", total_dolares);
    printf("Monto total en Bs: Bs. %.2f\n", total_bolivares);
}

void mostrarPacientesHospitalizados() {
    FILE *file = fopen(ARCHIVO_HOSPITALIZADOS, "rb");
    if (file) {
        Hospitalizacion h;
        printf("\nPacientes hospitalizados:\n");
        while (fread(&h, sizeof(Hospitalizacion), 1, file)) {
            printf("Cedula: %s, Nombre: %s, Edad: %d, Causa: %s, Dias: %d, Piso: %d, Habitacion: %d\n",
                   h.paciente.cedula, h.paciente.nombre, h.paciente.edad,
                   h.causa, h.dias_hospitalizado, h.piso, h.habitacion);
        }
        fclose(file);
    } else {
        printf("No hay datos de hospitalizacion disponibles.\n");
    }
}

void mostrarPacientesEmergencia() {
    FILE *file = fopen(ARCHIVO_EMERGENCIAS, "rb");
    if (file) {
        Emergencia e;
        printf("\nPacientes ingresados por emergencia el 10/03/2025 entre las 7 AM y las 7 PM:\n");
        while (fread(&e, sizeof(Emergencia), 1, file)) {
            if (strcmp(e.fecha, "10/03/2025") == 0 &&
                strcmp(e.hora, "07:00") >= 0 && strcmp(e.hora, "19:00") <= 0) {
                printf("Cedula: %s, Edad: %d\n", e.paciente.cedula, e.paciente.edad);
            }
        }
        fclose(file);
    } else {
        printf("No hay datos de emergencias disponibles.\n");
    }
}

int main() {
    generarPreciosServicios(); // Genera precios aleatorios para los servicios
    cargarPacientesDesdeArchivo(); // Carga pacientes desde el archivo

    int opcion;

    do {
        printf("\nMenu:\n");
        printf("1. Agregar Paciente\n");
        printf("2. Agregar Hospitalizacion\n");
        printf("3. Agregar Emergencia\n");
        printf("4. Calcular Monto Total de Servicios\n");
        printf("5. Mostrar Pacientes Hospitalizados\n");
        printf("6. Mostrar Pacientes Emergencia\n");
        printf("7. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        switch (opcion) {
            case 1:
                agregarPaciente();
                break;
            case 2:
                agregarHospitalizacion();
                break;
            case 3:
                agregarEmergencia();
                break;
            case 4:
                if (total_pacientes > 0) {
                    char cedula[20];
                    printf("Ingrese la cédula del paciente: ");
                    scanf("%s", cedula);
                    for (int i = 0; i < total_pacientes; i++) {
                        if (strcmp(pacientes[i].cedula, cedula) == 0) {
                            calcularMontoTotal(pacientes[i]);
                            break;
                        }
                    }
                } else {
                    printf("No hay pacientes registrados.\n");
                }
                break;
            case 5:
                mostrarPacientesHospitalizados();
                break;
            case 6:
                mostrarPacientesEmergencia();
                break;
            case 7:
                printf("Saliendo del programa.\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
                break;
        }
    } while (opcion != 7);

    return 0;
}
