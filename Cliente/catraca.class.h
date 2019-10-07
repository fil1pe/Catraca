size_t write_to_string(void *ptr, size_t size, size_t count, void *stream) {
	((string*)stream)->append((char*)ptr, 0, size*count);
	return size*count;
}

class catraca {
private:
	int id;
	vector<pessoa*> registros;
	
	void get_registros();
	
	void send_acesso(pessoa*);
	
	string data_hora(){
		time_t now = time(0);
		tm *ltm = localtime(&now);
		string resposta = "";
		resposta += to_string(1900 + ltm->tm_year) + "/";
		if (1 + ltm->tm_mon < 10) resposta += "0";
		resposta += to_string(1 + ltm->tm_mon) + "/";
		if (ltm->tm_mday < 10) resposta += "0";
		resposta += to_string(ltm->tm_mday) + " ";
		if (ltm->tm_hour < 10) resposta += "0";
		resposta += to_string(ltm->tm_hour) + ":";
		if (ltm->tm_min < 10) resposta += "0";
		resposta += to_string(ltm->tm_min);
		return resposta;
	}
public:
	catraca(int cid){ id = cid; }
	
	bool acessa(int);
	
	string get_nome(){
		switch(id){
			case 0:
				return "Principal";
			case 1:
				return "Sala dos professores";
			case 2:
				return "Sala dos servidores";
		}
		return "";
	}
};

bool catraca::acessa(int codigo){
	auto codstr = to_string(codigo);
	
	get_registros();
	
	for (auto i : registros)
		if (i->get_codigo() == codstr){
			if ((i->get_tipo() == "Aluno" && id != 0) || (i->get_tipo() == "Servidor" && id == 1) || (i->get_tipo() == "Professor" && id == 2))
				break;
			send_acesso(i);
			cout << "Acesso permitido para " << i->get_nome() << endl;
			return true;
		}
		
	cout << "Acesso negado\n";
	return false;
}

void catraca::get_registros(){
	CURL *curl = curl_easy_init();
	if(curl) {
		string readBuffer;
		
		curl_easy_setopt(curl, CURLOPT_URL, (host + ":" + port + "/getRegisters").c_str());
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		if(res != CURLE_OK){
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
			return;
		}
		
		registros = vector<pessoa*>(0);
		string delimiter = ";";
		size_t pos = 0;
		string token;
		while ((pos = readBuffer.find(delimiter)) != std::string::npos) {
			token = readBuffer.substr(0, pos) + ",";
			string delimiter1 = ",";
			size_t pos1 = 0;
			string token1;
			vector<string> pessoa_campos;
			while ((pos1 = token.find(delimiter1)) != std::string::npos) {
				pessoa_campos.push_back(token.substr(0, pos1));
				token.erase(0, pos1 + delimiter1.length());
			}
			
			if (pessoa_campos[3] == "Servidor")
				registros.push_back(new servidor(pessoa_campos[0], pessoa_campos[1], pessoa_campos[2]));
			else if (pessoa_campos[3] == "Professor")
				registros.push_back(new professor(pessoa_campos[0], pessoa_campos[1], pessoa_campos[2]));
			else
				registros.push_back(new aluno(pessoa_campos[0], pessoa_campos[1], pessoa_campos[2]));
			
			readBuffer.erase(0, pos + delimiter.length());
		}
	}
}

void catraca::send_acesso(pessoa *p){
	CURL *curl = curl_easy_init();
	if(curl) {
		string lista = "[{";
		lista += "\"codigo\": \"" + p->get_codigo() + "\", ";
		lista += "\"catraca\": \"" + get_nome() + "\", ";
		lista += "\"dateTime\": \"" + data_hora() + "\"";
		lista += "}]";
		
		lista = "list=" + lista;
		
		curl_easy_setopt(curl, CURLOPT_URL, (host + ":" + port + "/accessed").c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, lista.c_str() );//+","+get_nome()+","+data_hora()
		string readBuffer;
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_to_string);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
		CURLcode res = curl_easy_perform(curl);
		curl_easy_cleanup(curl);
		
		if(res != CURLE_OK)
			fprintf(stderr, "curl_easy_perform() failed: %s\n",curl_easy_strerror(res));
	}
}
