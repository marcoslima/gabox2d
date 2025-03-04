Namespaces do projeto:

1. GUI - Interface gráfica do programa
	O que é GUI?
	- Dezenhar qualquer coisa;
	- Obter parâmetros do usuário;
	
2. GA  - Algoritimo Genético
	O que é GA?
	- Definição de indivíduos (genes);
	- Cruzamento, seleção, etc;
3. Physics - Simulação física do programa
	O que é Physics?
	- Acho que tudo relacionado ao box2d, por enquanto. 
		Será que depois podemos trocar por um ODE, por exemplo? ehehehe


A classe CCar tem elementos de GA e elementos de GUI. Como resolver/separar?

Criamos uma classe que só tem GA e uma classe derivada desta que inclui o GUI.

Classe puramente GA: CCar
	Pode ser usado para evoluir os carros, mas não pode ser dezenhado.
	
Classe derivada com GUI: CGrCar
	
Indivíduo GA:
CGaCar.

Dezenhador do carro:
CGrCar.

Física do carro:
CPhysCar.

O carro completo:
CCar : public CGaCar, CGrCar, CPhysCar.



Elementos do carro, dos pontos de vista diferentes:

1. Gui
	- Roda: 
		point	centro	;
		double	raio	;
		bool	touch	;
		double	angle	;
		
	- Peso:
		point	centro	;
		double	raio	;
		bool	broke	;
		
	Roda	roda1;
	Roda	roda2;
	Peso	peso1;
	Peso	peso2;
	point	cm	 ;
	
2. Physics:
	Roda:
		point	centro		;
		double	raio		;
		double	densidade	;
		double	atrito		;
		double	elasticidade;
	
	Roda	roda1;
	Roda	roda2;
	Roda	peso1;
	Roda	peso2;
	
3. Ga:
			
	
