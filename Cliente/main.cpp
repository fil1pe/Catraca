/*
 * sudo apt install curl
 * sudo apt install libcurl4-gnutls-dev
 * g++ main.cpp -lcurl -o Cliente
 */
#include <iostream>
#include <vector>
#include <ctime>
#include <curl/curl.h>
using namespace std;

string host, port;

#include "pessoa.class.h"
#include "aluno.class.h"
#include "professor.class.h"
#include "servidor.class.h"
#include "catraca.class.h"

int main(int argc, char *argv[]){
	if (argc < 3){
		cout << "Uso: ./Cliente <host do servidor> <porta do servidor>\n\n";
		host = "http://localhost";
		port = "80";
	}else{
		host = argv[1];
		port = argv[2];
	}
	
	cout << "Selecione a catraca:\n";
	cout << "0 - Catraca principal (aceita alunos, professores e servidores)\n";
	cout << "1 - Catraca da sala dos professores\n";
	cout << "2 - Catraca da sala dos servidores\n";
	int catraca_id;
	cin >> catraca_id;
	if (catraca_id < 0 || catraca_id > 2){
		cout << "Opcao invalida\n";
		exit(1);
	}
	
	curl_global_init(CURL_GLOBAL_ALL);
	
	catraca *Catraca = new catraca(catraca_id);
	
	int codigo;
	cout << "Digite -1 para sair\n";
	cin >> codigo;
	while (codigo >= 0){
		Catraca->acessa(codigo);
		cin >> codigo;
	}
	
	return 0;
}
