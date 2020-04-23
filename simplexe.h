#ifndef SIMPLEXE_H
#define SIMPLEXE_H

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef unsigned int u32;

typedef struct contrainte_s
{
    double* coefficients;
    u32  var_in_base;

}contrainte_t;

typedef struct last_line_s
{
    double* coefficients;
}last_line_t;

typedef struct simplexe_info_s
{
    u32 nb_var;
    u32 nb_de_contrainte_inf_egale;

}simplexe_info_t;

u32 init_tab_simplexe(contrainte_t* tab_contraintes,
                      last_line_t* last_line,
                      simplexe_info_t* simplexe_info,
                      FILE* fptr_input);

u32 find_var_sortante(contrainte_t* tab_contraintes,
                      simplexe_info_t simplexe_info,
                      u32* indice_var_sortante,
                      u32* indice_var_entrante);

u32 find_var_entrante(last_line_t* last_line,
                      simplexe_info_t simplex_info,
                      u32* indice_var_entrante);

u32 normalise_pivot(contrainte_t* pivot,
                    u32 indice_var_entrante,
                    simplexe_info_t simplexe_info);

u32 pivot(contrainte_t* contrainte,
          u32 indice_var_entrante,
          contrainte_t pivot,
          simplexe_info_t simplexe_info);

u32 pivot_last_line(last_line_t* last_line,
                    u32 indice_var_entrante,
                    contrainte_t pivot,
                    simplexe_info_t simplexe_info);

u32 maj_tab_simplexe(contrainte_t* tab_contraintes,
                     last_line_t* last_line,
                     contrainte_t le_pivot,
                     u32 indice_var_sortante,
                     u32 indice_var_entrante,
                     simplexe_info_t simplex_info);

u32 test_fin_algo(last_line_t last_line,
                  simplexe_info_t* simplex_info);

u32 write_solution(contrainte_t* tab_contraintes,
                   last_line_t last_line,
                   simplexe_info_t simplexe_info,
                   FILE* fptr_output);
#endif