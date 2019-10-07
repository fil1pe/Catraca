class aluno : public virtual pessoa {
public:
	aluno(string codigo, string nome, string cpf) : pessoa(codigo, nome, cpf){}
	
	string get_tipo(){ return "Aluno"; }
};
