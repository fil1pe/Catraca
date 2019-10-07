class professor : public virtual pessoa {
public:
	professor(string codigo, string nome, string cpf) : pessoa(codigo, nome, cpf){}
	
	string get_tipo(){ return "Professor"; }
};
