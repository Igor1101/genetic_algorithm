//============================================================================
// Name        : genetic.cpp
// Author      : Igor
// Version     :
// Description : genetic algorithm
//============================================================================

#include <cstdio>
#include <cstdlib>
#include <new>
#include <algorithm>    // std::sort
#include <array>
#include "getch.h"
#include "RNG.h"

#define DEBUG 1

#if DEBUG
#define pr_debug(...) printf(__VA_ARGS__)
#else
#define pr_debug(...)
#endif
class genotype {
public:
	int x1;
	int x2;
	int x3;
	int delta;
	double Pfather;
	void generate(int y)
	{
		RNG_init();
		if(y>1) {
			x1 = RNG.get_int(1, y/2);
			x2 = RNG.get_int(1, y/2);
			x3 = RNG.get_int(1, y/2);
		} else {
			x1 = RNG.get_int(y/2, 1);
			x2 = RNG.get_int(y/2, 1);
			x3 = RNG.get_int(y/2, 1);
		}
	}
	void mutate()
	{
		RNG_init();
		switch(RNG.get_int(0, 6)) {
		case 0:
			x1 += 1;
			break;
		case 1:
			x2 += 1;
			break;
		case 2:
			x3 += 1;
			break;
		case 3:
			x1 -= 1;
			break;
		case 4:
			x2 -= 1;
			break;
		case 5:
			x3 -= 1;
			break;
		case 6:
			break;
		default:
			break;
		}
	}
	void print()
	{
		printf(":Pfather=%f\tdelta=%d\tx1=%d\tx2=%d\tx3=%d\n", Pfather, delta, x1, x2, x3);
	}
};
class genetic {
private:
	static constexpr int GENS = 5;
	int y;
	int a;
	int b;
	int c;
	int d;
	std::array <genotype, GENS> children;
	std::array <genotype, GENS> fathers;
	static bool cmp_fathers(genotype f1, genotype f2) {
		return f1.Pfather > f2.Pfather;
	}
public:
	genetic(int y, int a, int b, int c) {
		this->y = y;
		this->a = a;
		this->b = b;
		this->c = c;
	}
	int f(genotype a)
	{
		return f(a.x1, a.x2, a.x3);
	}
	int f(int x1, int x2, int x3)
	{
		return a*x1 + b*x2 + c*x3 ;
	}
	bool result(int x1, int x2, int x3)
	{
		return f(x1,x2,x3) == y;
	}
	void generate_fathers()
	{
		RNG_init();
		for(int i=0; i<GENS; i++) {
			fathers[i].generate(y);
		}
	}
	/* if found solution, return it ! */
	genotype* fitness()
	{
		for(int i=0; i<GENS; i++) {
			fathers[i].delta = abs(f(fathers[i]) - y);
			if(fathers[i].delta == 0)
				return &fathers[i];
		}
		return NULL;
	}
	void Pcalc()
	{
		double ndsum=0.0;
		for(int i=0; i<GENS ; i++) {
			ndsum += 1 / (double)fathers[i].delta;
		}
		for(int i=0; i<GENS; i++) {
			fathers[i].Pfather = (1 / (double)fathers[i].delta) / ndsum;
		}
	}
	genotype cross(genotype g1, genotype g2)
	{
		RNG_init();
		genotype result;
		int cross_point = RNG.get_int(0, 1);
		switch(cross_point) {
		case 0:
			result.x1 = g1.x1;
			result.x2 = g2.x2;
			result.x3 = g2.x3;
			break;
		case 1:
			result.x1 = g1.x1;
			result.x2 = g1.x2;
			result.x3 = g2.x3;
			break;
		}
		return result;
	}
	// return selected parent
	genotype selection(void)
	{
		float rand = RNG.get_float(0, 1);
		float sumPfather = 0;
		for(int i=0; i<GENS; i++) {
			sumPfather += fathers[i].Pfather;
			if(rand < sumPfather) {
				return fathers[i];
			}
		}
		puts("something wrong with selection");
		return fathers[0];
	}
	// return most approximated result
	genotype calculate_children()
	{
		// sort fathers
		//std::sort(fathers.begin(), fathers.end(), cmp_fathers);
		// get from best to worst ones
		for(int i=0; i<GENS; i++) {
			genotype gen1 = selection();
			genotype gen2 = selection();
			pr_debug("selected:");
			if(DEBUG) {
				gen1.print();
				gen2.print();
			}
			children[i] = cross(gen1, gen2);
		}
		return fathers[0];
	}
	void mutate_children()
	{
		RNG_init();
		int times = RNG.get_int(1, GENS);
		for(int i=0; i<times; i++) {
			//select any
			children[RNG.get_int(0, GENS-1)].mutate();
		}
	}
	void print_children()
	{
		printf("children:\n");
		for(int i=0; i<GENS; i++) {
			printf("gen %i ", i);
			children[i].print();
			puts("");
		}
	}
	void print_fathers()
	{
		printf("fathers:\n");
		for(int i=0; i<GENS; i++) {
			printf("gen %i ", i);
			fathers[i].print();
			puts("");
		}
	}
	void copy_children_fathers() {
		for(int i=0; i<GENS; i++) {
			fathers[i] = children[i];
		}
	}
};
int main(void) {
	puts("genetic algorithm");
	puts(" ax1+bx2+cx3=y ");
	puts("input a b c d y");
	printf("-> ");
	int a,b,c,y;
	scanf("%d%d%d%d", &a,&b,&c,&y);
	genetic genalg = genetic(y, a, b, c);
	genalg.generate_fathers();
	int iter=1;
	int itermax=1000;
	for(;;iter++) {
		pr_debug("iter:%d, :", iter);
		genotype* mayberesult = genalg.fitness();
		if(mayberesult != NULL) {
			printf("found result to equation at iteration %d:\n", iter);
			mayberesult->print();
			return EXIT_SUCCESS;
		}
		genalg.Pcalc();
		if(DEBUG)
			genalg.print_fathers();
		genotype apprx = genalg.calculate_children();
		if(iter == itermax) {
			itermax *= 10;
			char ch;
			printf("approximated result:");
			apprx.print();
			printf("operation took %d iterations, continue?(Y/N)\n", iter);
			/* use system call to make terminal send all keystrokes directly to stdin */
			while(1) {
				set_conio_terminal_mode();
				ch=getch();
				reset_terminal_mode();
				putchar(ch);
				if(ch == 'Y') {
					puts("continuing");
				 	break;
				}
				else if(ch == 'N')
					return EXIT_FAILURE;
				else
					printf("(Y/N)");
			}
			/* use system call to set terminal behaviour to more normal behaviour */
		}
		if(DEBUG)
			genalg.print_children();
		pr_debug("children mutation");
		genalg.mutate_children();
		if(DEBUG)
			genalg.print_children();
		genalg.copy_children_fathers();
	}
	return EXIT_SUCCESS;
}
