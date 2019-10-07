class servidor : public virtual pessoa {
public:
	servidor(string codigo, string nome, string cpf) : pessoa(codigo, nome, cpf){}
	
	string get_tipo(){ return "Servidor"; }
};
