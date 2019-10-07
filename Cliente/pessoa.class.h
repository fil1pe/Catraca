class pessoa {
private:
	string codigo, nome, cpf;
public:
	pessoa(string codigo, string nome, string cpf){
		pessoa::codigo = codigo;
		pessoa::nome = nome;
		pessoa::cpf = cpf;
	}
	
	// getters
	string get_codigo(){ return codigo; }
	string get_nome(){ return nome; }
	string get_cpf(){ return cpf; }
	virtual string get_tipo() = 0;
};
