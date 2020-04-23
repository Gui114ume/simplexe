#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "simplexe.h"

/*
 * On considere que le fichier d'entree ne comprends que des contraintes en <= !
 */

u32 init_tab_simplexe(contrainte_t* tab_contraintes,
                      last_line_t* last_line,
                      simplexe_info_t* simplexe_info,
                      FILE* fptr_input)
{
    char* the_string = malloc(10 * sizeof(char) + sizeof(char));

    for(int i = 0; i < simplexe_info->nb_de_contrainte_inf_egale; i++)
    {
        for(int j = 0 ; j < simplexe_info->nb_var ; j++)
        {
            fgets(the_string, 10, fptr_input);
            tab_contraintes[i].coefficients[j] = atoi(the_string);

        }
        for(int j = simplexe_info->nb_var ; j < simplexe_info->nb_var + simplexe_info->nb_de_contrainte_inf_egale ; j++)
        {
            if( j == i + simplexe_info->nb_var )
            {
                tab_contraintes[i].coefficients[j] = 1;
                tab_contraintes[i].var_in_base     = j;
            }
            else
                tab_contraintes[i].coefficients[j] = 0;
        }
        fgets(the_string, 10, fptr_input);
        tab_contraintes[i].coefficients[simplexe_info->nb_de_contrainte_inf_egale + simplexe_info->nb_var] = atoi(the_string);
    }
    for(int i = 0 ; i < simplexe_info->nb_var ; i++)
    {
        fgets(the_string, 10, fptr_input);
        last_line->coefficients[i] = atoi(the_string) * (-1);
    }
    for(int i = simplexe_info->nb_var ; i < simplexe_info->nb_var + simplexe_info->nb_de_contrainte_inf_egale + 1; i++)
        last_line->coefficients[i] = 0;

    free(the_string);
    return 1;
}

u32 find_var_sortante(contrainte_t* tab_contraintes,
                      simplexe_info_t simplexe_info,
                      u32* indice_var_sortante,
                      u32* indice_var_entrente)
{
    //plus petit rapport positif......
    double rapport_min = 100000;
    u32 indice_rapport_min = 0;
    for(int i = 0 ; i < simplexe_info.nb_de_contrainte_inf_egale ; i++)
    {
        printf("%lf\n",tab_contraintes[i].coefficients[simplexe_info.nb_de_contrainte_inf_egale + simplexe_info.nb_var]);
        //printf("%lf\n",tab_contraintes[i].coefficients[*indice_var_entrente]);
        if( (tab_contraintes[i].coefficients[simplexe_info.nb_de_contrainte_inf_egale + simplexe_info.nb_var] / tab_contraintes[i].coefficients[*indice_var_entrente] < rapport_min) && (tab_contraintes[i].coefficients[*indice_var_entrente] > 0) )
        {
            indice_rapport_min = i;
            rapport_min = tab_contraintes[i].coefficients[simplexe_info.nb_de_contrainte_inf_egale + simplexe_info.nb_var] / tab_contraintes[i].coefficients[*indice_var_entrente];
        }
    }
    *indice_var_sortante = indice_rapport_min;
    return 1;
}

u32 find_var_entrante(last_line_t* last_line,
                      simplexe_info_t simplex_info,
                      u32* indice_var_entrante)
{
    //element le plus negatif
    double max_negatif = 0;
    for(int i = 0 ; i < simplex_info.nb_de_contrainte_inf_egale + simplex_info.nb_var ; i++)
    {
        if (last_line->coefficients[i] < max_negatif)
        {
            max_negatif = last_line->coefficients[i];
            *indice_var_entrante = i;
        }
    }
    return 1;
}

u32 normalise_pivot(contrainte_t* pivot,
                    u32 indice_var_entrante,
                    simplexe_info_t simplexe_info)
{
    double tmp = pivot->coefficients[indice_var_entrante];
    for(int i = 0; i < simplexe_info.nb_de_contrainte_inf_egale + simplexe_info.nb_var + 1 ; i++)
    {
        pivot->coefficients[i] = pivot->coefficients[i] / tmp;
    }
    pivot->var_in_base = indice_var_entrante;
    return 1;
}

//agit sur une seule contrainte
u32 pivot(contrainte_t* contrainte,
          u32 indice_var_entrante,
          contrainte_t pivot,
          simplexe_info_t simplexe_info)
{
    double coef = contrainte->coefficients[indice_var_entrante];
    for(int i = 0 ; i < simplexe_info.nb_var + simplexe_info.nb_de_contrainte_inf_egale + 1; i++)
    {
        contrainte->coefficients[i] -= coef * pivot.coefficients[i];

    }
    return 1;
}

u32 pivot_last_line(last_line_t* last_line,
                    u32 indice_var_entrante,
                    contrainte_t pivot,
                    simplexe_info_t simplexe_info)
{
    double coef = last_line->coefficients[indice_var_entrante];
    for(int i = 0 ; i < simplexe_info.nb_var + simplexe_info.nb_de_contrainte_inf_egale + 1; i++)
    {
        last_line->coefficients[i] -= coef * pivot.coefficients[i];
    }
    return 1;
}

u32 maj_tab_simplexe(contrainte_t* tab_contraintes,
                     last_line_t* last_line,
                     contrainte_t le_pivot,
                     u32 indice_var_sortante,
                     u32 indice_var_entrante,
                     simplexe_info_t simplex_info)
{
    for(int i = 0; i < simplex_info.nb_de_contrainte_inf_egale ; i++)
    {


        if(i == indice_var_sortante)
            continue;
        else
        {
            pivot(  &tab_contraintes[i],
                    indice_var_entrante,
                    tab_contraintes[indice_var_sortante],
                    simplex_info);

        }
    }
    pivot_last_line(last_line,
                    indice_var_entrante,
                    tab_contraintes[indice_var_sortante],
                    simplex_info);
    return 1;
}

//renvoie un booleen
u32 test_fin_algo(last_line_t last_line,
                  simplexe_info_t* simplexe_info)
{
    //algo terminé     : return 0
    //algo non terminé : return 1
    for(int i = 0 ; i < simplexe_info->nb_var + simplexe_info->nb_de_contrainte_inf_egale ; i++)
    {
        if(last_line.coefficients[i] < 0)
            return 1;
    }
    return 0;
}

u32 write_solution(contrainte_t* tab_contraintes,
                   last_line_t last_line,
                   simplexe_info_t simplexe_info,
                   FILE* fptr_output)
{
    for(int i = 0 ; i < simplexe_info.nb_de_contrainte_inf_egale ; i++)
    {
            fprintf(fptr_output, "x%d = %lf\n",i,tab_contraintes[i].coefficients[simplexe_info.nb_var + simplexe_info.nb_de_contrainte_inf_egale]);

    }
    fprintf(fptr_output, "valeur optimale = %lf\n", last_line.coefficients[simplexe_info.nb_de_contrainte_inf_egale + simplexe_info.nb_var]);
    fprintf(fptr_output, "last line %lf %lf %lf %lf\n",last_line.coefficients[0],last_line.coefficients[1],last_line.coefficients[2],last_line.coefficients[3]);
    return 1;
}

int main(int argc, char* argv[])
{
    if(argc != 3)
    {
        printf("usage : %s [fichier_entree] [fichier_sortie]\n",argv[0]);fflush(stdout);
        exit(-1);
    }
    FILE* fptr_input  = fopen(argv[1], "r");
    FILE* fptr_output = fopen(argv[2], "w");

    simplexe_info_t* simplexe_info = malloc(sizeof(simplexe_info_t));
    contrainte_t* tab_contrainte = NULL;
    last_line_t* last_line = NULL;

    char* str_nb_var         = malloc(10 * sizeof(char) + sizeof(char));
    char* str_nb_contraintes = malloc(10 * sizeof(char) + sizeof(char));

    fgets(str_nb_var        , 10, fptr_input);
    fgets(str_nb_contraintes, 10, fptr_input);

    int nb_var = atoi(str_nb_var);
    int nb_contraintes = atoi(str_nb_contraintes);

    free(str_nb_contraintes);
    free(str_nb_var);

    simplexe_info->nb_de_contrainte_inf_egale = nb_contraintes;
    simplexe_info->nb_var = nb_var;

    tab_contrainte = malloc(nb_contraintes * sizeof(contrainte_t));
    for(int i = 0; i < nb_contraintes ; i++)
        tab_contrainte[i].coefficients = malloc((nb_var + nb_contraintes + 1) * sizeof(double));

    last_line      = malloc(sizeof(last_line_t));
    last_line->coefficients = malloc((nb_var + nb_contraintes + 1) * sizeof(double));

    init_tab_simplexe(tab_contrainte,
                      last_line,
                      simplexe_info,
                      fptr_input);
    //printf("%lf %lf %lf\n",tab_contrainte[0].coefficients[5],tab_contrainte[1].coefficients[5],tab_contrainte[2].coefficients[5]);

    test_fin_algo(*last_line,
                  simplexe_info);

    u32 booleen = 1;
    u32 indice_sortant = 0;
    u32 indice_entrant = 0;

    while(booleen)
    {
        //seg fault ici

        find_var_entrante(last_line,
                          *simplexe_info,
                          &indice_entrant);

        find_var_sortante(tab_contrainte,
                        *simplexe_info,
                        &indice_sortant,
                        &indice_entrant);


        printf("entrant %d %d\n",indice_entrant, indice_sortant);
        normalise_pivot(&tab_contrainte[indice_sortant],
                        indice_entrant,
                        *simplexe_info);
        printf("%lf %lf %lf\n",tab_contrainte[0].coefficients[5],tab_contrainte[1].coefficients[5],tab_contrainte[2].coefficients[5]);

        maj_tab_simplexe(tab_contrainte,
                         last_line,
                         tab_contrainte[indice_sortant],
                         indice_sortant,
                         indice_entrant,
                         *simplexe_info);
        //printf("%lf %lf %lf\n",tab_contrainte[0].coefficients[5],tab_contrainte[1].coefficients[5],tab_contrainte[2].coefficients[5]);

        booleen = test_fin_algo(*last_line,
                                simplexe_info);
        //printf("%lf %lf %lf\n",tab_contrainte[0].coefficients[5],tab_contrainte[1].coefficients[5],tab_contrainte[2].coefficients[5]);

    }
    write_solution(tab_contrainte,
                   *last_line,
                   *simplexe_info,
                   fptr_output);

    fclose(fptr_input);
    fclose(fptr_output);
    return EXIT_SUCCESS;
}