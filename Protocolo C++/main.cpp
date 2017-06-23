#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define DISPONIBLE 0

using namespace std;

int main()
{
    int mensaje_proceso = 1;
    int tamano = sizeof(int);

    // MEMORIA ANTERIOR
    key_t key_ant = 5690;

	int id_mem_ant;
	id_mem_ant = shmget(key_ant, tamano, IPC_CREAT | 0666);

    int* mem_ant;
    mem_ant = (int*)shmat(id_mem_ant, 0, 0);

	*mem_ant = 0;

    // MEMORIA SIGUIENTE
    key_t key_sig = 5691;

	int id_mem_sig;
	id_mem_sig = shmget(key_sig, tamano, IPC_CREAT | 0666);

    int* mem_sig;
    mem_sig = (int*)shmat(id_mem_sig, 0, 0);

	*mem_sig = 0;

    // ALERTA ALEATORIA PARA ENVIAR UN MENSAJE
	srand(time(NULL));
	int modulo = 10;
    int alerta = rand() % modulo;

    bool mensaje_pendiente_flag = false;
    bool mensaje_enviado = false;
    vector<int> mensaje_pendiente;

    bool entrada;
    cin >> entrada;

    // ESCRIBIENDO EN LA MEMORIA COMPARTIDA
	while (1){
	    cout << endl << "Esperando ..." << endl;
        cout << "Modulo: " << modulo << endl;
        cout << "Alerta: " << alerta << endl;
        cout << "Ant[Recuerso]: " << *mem_ant << endl;
        cout << "Sig[Recuerso]: " << *mem_sig << endl << endl;

        // ENVIANDO MENSAJE
        if (mensaje_pendiente_flag){
            cout << "    [Reenviando mensaje] Veficando" << endl;

            if (*mem_sig == DISPONIBLE){
                cout << "    [Reenviando mensaje] Recurso disponible" << endl;

                *mem_sig = mensaje_pendiente.back();
                mensaje_pendiente.pop_back();

                cout << "    [Reenviando mensaje] Mensaje enviado" << endl << endl;

                if (mensaje_pendiente.size() == 0)
                    mensaje_pendiente_flag = false;
            }
            else{
                cout << "    [Reenviando mensaje] Recurso NO disponible" << endl;
                cout << "    [Cola de mensajes] ";
                for (int i = 0; i < mensaje_pendiente.size(); i++)
                    cout << mensaje_pendiente[i] << " ";
                cout << endl << endl;
            }
        }

        if (alerta == 0 && mensaje_enviado == false){
            mensaje_enviado = true;
            modulo = 10;
            cout << "[Enviando mensaje] Veficando" << endl;

            if (*mem_sig == DISPONIBLE){
                cout << "[Enviando mensaje] Recurso disponible" << endl;

                *mem_sig = mensaje_proceso;

                cout << "[Enviando mensaje] Mensaje enviado" << endl << endl;
            }
            else{
                mensaje_pendiente_flag = true;
                mensaje_pendiente.push_back(mensaje_proceso);
                cout << "[Enviando mensaje] Recurso NO disponible" << endl << endl;
            }
        }

        if (modulo == 1)
            modulo = 10;

        //LEYENDO MENSAJE
        if (*mem_ant != DISPONIBLE){
            if (*mem_ant != mensaje_proceso){
                cout << "---------------------------------------" << endl;
                cout << "[Leyendo mensaje] Mensaje del proceso " << *mem_ant << endl;
                cout << "---------------------------------------" << endl << endl;

                int mensaje_leido = *mem_ant;
                *mem_ant = 0;

                cout << "    [Pasando mensaje] Veficando" << endl;

                if (*mem_sig == DISPONIBLE){
                    cout << "    [Pasando mensaje] Recurso disponible" << endl;

                    *mem_sig = mensaje_leido;

                    cout << "    [Pasando mensaje] Mensaje enviado" << endl << endl;
                }
                else{
                    mensaje_pendiente_flag = true;
                    mensaje_pendiente.push_back(mensaje_leido);
                    cout << "    [Pasando mensaje] Recurso NO disponible" << endl << endl;
                }
            }
            else{
                mensaje_enviado = false;
                *mem_ant = 0;

                cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl;
                cout << "------------------------------------" << endl;
                cout << "[Pasando mensaje] Mensaje completado" << endl;
                cout << "------------------------------------" << endl;
                cout << "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx" << endl << endl;
            }
        }

        modulo--;
        alerta = rand() % modulo;
        usleep(2000000);
    }

	return 0;
}
