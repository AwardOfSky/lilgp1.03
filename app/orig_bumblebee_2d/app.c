/*  lil-gp Genetic Programming System, version 1.0, 11 July 1995
 *  Copyright (C) 1995  Michigan State University
 * 
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of version 2 of the GNU General Public License as
 *  published by the Free Software Foundation.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *  
 *  Douglas Zongker       (zongker@isl.cps.msu.edu)
 *  Dr. Bill Punch        (punch@isl.cps.msu.edu)
 *
 *  Computer Science Department
 *  A-714 Wells Hall
 *  Michigan State University
 *  East Lansing, Michigan  48824
 *  USA
 *  
 */

#include <stdio.h>
#include <math.h>

#include <lilgp.h>

time_t tstart;

globaldata g;

#define APP_DEFAULT_LAWN_WIDTH  10 
#define APP_DEFAULT_LAWN_HEIGHT 10 

#define APP_FITNESS_CASES 10 
#define APP_DEFAULT_NUM_OF_FLOWERS 10 
static vector app_fitness_cases[APP_FITNESS_CASES][APP_DEFAULT_NUM_OF_FLOWERS];
static int    fvisited[APP_FITNESS_CASES][APP_DEFAULT_NUM_OF_FLOWERS];

int app_build_function_sets ( void )
{
     function_set *fset;
     int *tree_map;
     char *tree_name[3];
     int ret;

     function sets[3][10] =

     /*** RPB ***/
     { { { f_bee,        NULL,     NULL,     0, "(bee)",  TERM_NORM, -1, 0 },
         { f_nextflower, NULL,     NULL,     0, "(nf)",   TERM_NORM, -1, 0 },
         { NULL,         f_vecgen, f_vecstr, 0, "rv",   TERM_ERC,  -1, 0 },
         { f_vadd,       NULL,     NULL,     2, "vadd",  FUNC_DATA, -1, 0 },
         { f_vsub,       NULL,     NULL,     2, "vsub",  FUNC_DATA, -1, 0 },
         { f_gox,        NULL,     NULL,     1, "gox",   FUNC_DATA, -1, 0 },
         { f_goy,        NULL,     NULL,     1, "goy",   FUNC_DATA, -1, 0 },
         { f_prog2,      NULL,     NULL,     2, "prog2", FUNC_DATA, -1, 0 },
         { NULL,         NULL,     NULL,    -1, "adf0",  EVAL_DATA,  1, 0 } },

            /*** ADF0 ***/
       { { f_vadd,       NULL,     NULL,     2, "vadd",  FUNC_DATA, -1, 0 },
         { f_vsub,       NULL,     NULL,     2, "vsub",  FUNC_DATA, -1, 0 },
         { f_gox,        NULL,     NULL,     1, "gox",   FUNC_DATA, -1, 0 },
         { f_goy,        NULL,     NULL,     1, "goy",   FUNC_DATA, -1, 0 },
         { f_prog2,      NULL,     NULL,     2, "prog2", FUNC_DATA, -1, 0 },
         { NULL,         NULL,     NULL,     0, "a0",  TERM_ARG,   0, 0 },
         { f_bee,        NULL,     NULL,     0, "(bee)",   TERM_NORM, -1, 0 },
         { f_nextflower, NULL,     NULL,     0, "(nf)",    TERM_NORM, -1, 0 },
         { NULL,         f_vecgen, f_vecstr, 0, "rv",    TERM_ERC,  -1, 0 } },


            /*** non-ADF function set ***/
       { { f_bee,        NULL,     NULL,     0, "(bee)",   TERM_NORM, -1, 0 },
         { f_nextflower, NULL,     NULL,     0, "(nf)",    TERM_NORM, -1, 0 },
         { NULL,         f_vecgen, f_vecstr, 0, "Rvm",   TERM_ERC,  -1, 0 },
         { f_vadd,       NULL,     NULL,     2, "vadd",  FUNC_DATA, -1, 0 },
         { f_vsub,       NULL,     NULL,     2, "vsub",  FUNC_DATA, -1, 0 },
         { f_gox,        NULL,     NULL,     1, "gox",   FUNC_DATA, -1, 0 },
         { f_goy,        NULL,     NULL,     1, "goy",   FUNC_DATA, -1, 0 },
         { f_prog2,      NULL,     NULL,     2, "prog2", FUNC_DATA, -1, 0 } } };

     binary_parameter ( "app.use_adfs", 1 );
     if ( atoi ( get_parameter ( "app.use_adfs" ) ) )
     {

          fset = (function_set *)MALLOC ( 2 * sizeof ( function_set ) );
          fset[0].size = 9;
          fset[0].cset = sets[0];
          fset[1].size = 9;
          fset[1].cset = sets[1];
          
          tree_map = (int *)MALLOC ( 2 * sizeof ( int ) );
          tree_map[0] = 0;
          tree_name[0] = "RPB";
          tree_map[1] = 1;
          tree_name[1] = "ADF0";
          
          ret = function_sets_init ( fset, 2, tree_map, tree_name, 2 );
     }
     else
     {
          fset = (function_set *)MALLOC ( 1 * sizeof ( function_set ) );
          fset[0].size = 8;
          fset[0].cset = sets[2];
          
          tree_map = (int *)MALLOC ( 1 * sizeof ( int ) );
          tree_map[0] = 0;
          tree_name[0] = "TREE";
          
          ret = function_sets_init ( fset, 1, tree_map, tree_name, 1 );
     }

     FREE ( tree_map );
     FREE ( fset );

     return ret;
}

void app_eval_fitness ( individual *ind )
{
  int i, j, k;
  DATATYPE value;

  set_current_individual ( ind );

  g.xpos = 0.0;
  g.ypos = 0.0;
  g.movecount = 0;
  g.abort = 0;

  for ( i = 0; i < APP_FITNESS_CASES; ++i )
  {
    for(int f = 0; f < g.nNumFlowers; f++)
    {
      fvisited[i][f] = 0;
    }
  }

  for ( i = 0; i < APP_FITNESS_CASES; ++i )
  {
    for(int f = 0; f < g.nNumFlowers; f++)
    {
      g.pfl[i].x = app_fitness_cases[i][f].x;
      g.pfl[i].y = app_fitness_cases[i][f].y;
      g.pvis[i] = fvisited[i][f];
    }

    evaluate_tree ( ind->tr[0].data, 0 );

    for(int f = 0; f < g.nNumFlowers; f++)
    {
      fvisited[i][f] = g.pvis[i];
    }
  }

  k = 0;
  for ( i = 0; i < APP_FITNESS_CASES; ++i )
  {
    for(int f = 0; f < g.nNumFlowers; f++)
    { 
      k += fvisited[i][f];
    }
  }

  ind->hits = k;
  ind->r_fitness = k;
        
  /* compute the standardized and raw fitness. */
  ind->s_fitness = (g.nNumFlowers * APP_FITNESS_CASES) - ind->r_fitness;
  ind->a_fitness = 1/(1+ind->s_fitness);

     /* always leave this line in. */
  ind->evald = EVAL_CACHE_VALID;
}

int app_end_of_evaluation ( int gen, multipop *mpop, int newbest, popstats *gen_stats, popstats *run_stats )
{
  int i, j, tr;
  char fname[BUFSIZ];
  static FILE* fptr=NULL;
  static FILE* fptrbind=NULL;
  int numPops = mpop->size;
  const char* basefile = get_parameter( "output.basename" );
  const int popSize=atoi(get_parameter("pop_size"));
  population** pops = mpop->pop;

  binary_parameter ( "app.save_pop" , 0);

  if ( atoi ( get_parameter ( "app.save_pop" ) ) == 1)
  {
    /* open pop file and print individuals in population */
    strcpy(fname,basefile);
    strcat(fname,".pop");
    if ((fptr=fopen(fname,"a"))==0)
    {
      fprintf(stderr,"Could not open %s to write weights\n",fname);
      exit(1);
    }

    for (i=0; i<numPops; i++) // pop
    {
      for (j=0; j<popSize; j++) // individual
      {
        fprintf(fptr, "Gen %d PopNum %d IndNum %d\n", gen, i, j);
        int tr = tree_count-1;
        for(; tr >= 0; tr--) // tree
        {
          fprintf(fptr, " %s", tree_map[tr].name);
          //pretty_print_tree ( pops[i]->ind[j].tr[tr].data, fptr);
          print_tree ( pops[i]->ind[j].tr[tr].data, fptr);
          //fprintf(fptr, "%s", "\n");
        }
        fflush(fptr); 
      }
    }
  }

  /* save best individual  */
  strcpy(fname,basefile);
  strcat(fname,".bind");
  if ((fptrbind=fopen(fname,"w"))==0)
  {
    fprintf(stderr,"Could not open %s to write best individual.\n",fname);
    exit(1);
  }

  fprintf(fptrbind, "%s\n", "(ql:quickload \"read-csv\" :silent t)");
  fprintf(fptrbind, "%s\n", "(ql:quickload \"cl-strings\" :silent t)");
  fprintf(fptrbind, "%s\n", "(use-package :read-csv)");
  fprintf(fptrbind, "%s\n", "(use-package :cl-strings)");
  fprintf(fptrbind, "%s\n", ";;read in csv file");
  fprintf(fptrbind, "%s\n", "(defvar flowerloc)");
  fprintf(fptrbind, "%s\n", "(defvar fv)");
  fprintf(fptrbind, "%s\n", "(defvar lc)");
  fprintf(fptrbind, "%s\n", "(defvar row)");
  fprintf(fptrbind, "%s\n", "(defvar rc)");
  fprintf(fptrbind, "%s\n", "(defvar fc)");
  fprintf(fptrbind, "%s\n", "(defvar xx)");
  fprintf(fptrbind, "%s\n", "(defvar yy)");
  fprintf(fptrbind, "%s\n", "(defvar fcn)");
  fprintf(fptrbind, "%s\n", "(defvar xxn)");
  fprintf(fptrbind, "%s\n", "(defvar yyn)");
  fprintf(fptrbind, "%s\n", "(defvar pt)");
  fprintf(fptrbind, "%s\n", "(defvar g)");
  fprintf(fptrbind, "%s\n", "(defvar a0)");
  fprintf(fptrbind, "%s\n", "(setf lc 0)");
  fprintf(fptrbind, "%s\n", "(setf rc 0)");
  fprintf(fptrbind, "%s\n", "(defstruct vec (x 0) (y 0)) ; define structure");
  fprintf(fptrbind, "%s\n", ";; Loading fitness cases .....");
  fprintf(fptrbind, "%s\n", "(defstruct gdat ");
  fprintf(fptrbind, "%s\n", " (pfl (make-array '(10 10) :initial-element (make-vec :x 0.0 :y 0.0)))");
  fprintf(fptrbind, "%s\n", " (pvis(make-array '(10) :initial-element 0))");
  fprintf(fptrbind, "%s\n", " (nNumFlowers 10) ;int nNumFlowers;");
  fprintf(fptrbind, "%s\n", " (lawn_width 10)  ;float lawn_width;");
  fprintf(fptrbind, "%s\n", " (lawn_height 10) ;float lawn_height;");
  fprintf(fptrbind, "%s\n", " (xpos 0.0)       ;float xpos;");
  fprintf(fptrbind, "%s\n", " (ypos 0.0)       ;float ypos;");
  fprintf(fptrbind, "%s\n", " (curfc 0)        ; current fitness case");
  fprintf(fptrbind, "%s\n", " ;int movecount;");
  fprintf(fptrbind, "%s\n", " ;int abort;");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(setf g (make-gdat :nNumFlowers 10 :lawn_width 10 :xpos 0 :ypos 0)) ");
  fprintf(fptrbind, "%s\n", "(setf fv (with-open-file (s \"./file.dat\") (parse-csv s)))");
  fprintf(fptrbind, "%s\n", ";; load flowers locations in to fitness cases array");
  fprintf(fptrbind, "%s\n", "(loop for row in fv");
  fprintf(fptrbind, "%s\n", "  do(setf fc (nth 0 row))");
  fprintf(fptrbind, "%s\n", "  do(setf fcn (parse-number fc))");
  fprintf(fptrbind, "%s\n", "  do(setf xx (nth 1 row))");
  fprintf(fptrbind, "%s\n", "  do(setf xxn (parse-number xx))");
  fprintf(fptrbind, "%s\n", "  do(setf yy (nth 2 row))");
  fprintf(fptrbind, "%s\n", "  do(setf yyn (parse-number yy))");
  fprintf(fptrbind, "%s\n", "  ;;do(format t \"~a,~a,~a~%\"  fc xx yy)");
  fprintf(fptrbind, "%s\n", "  (setf pt (make-vec :x xxn :y yyn)) ;  set value of slots of var ");
  fprintf(fptrbind, "%s\n", "  do(setf (aref (gdat-pfl g) fcn rc) pt)");
  fprintf(fptrbind, "%s\n", "  do(setf rc (+ rc 1))");
  fprintf(fptrbind, "%s\n", "  do(if (> rc 9) (setf rc 0) )");
  fprintf(fptrbind, "%s\n", "  do(setf lc (+ lc 1))");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", ";; functions.....");
  fprintf(fptrbind, "%s\n", "(defun vadd (a b)");
  fprintf(fptrbind, "%s\n", " (make-vec :x (+ (vec-x a) (vec-x b)) :y (+ (vec-y a) (vec-y b)))");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(defun vsub (a b)");
  fprintf(fptrbind, "%s\n", " (make-vec :x (- (vec-x a) (vec-x b)) :y (- (vec-y a) (vec-y b)))");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(defun gox (a)");
  fprintf(fptrbind, "%s\n", " (setf (gdat-xpos g) (+ (gdat-xpos g)))");
  fprintf(fptrbind, "%s\n", " ;;check if bee is near a flower");
  fprintf(fptrbind, "%s\n", " (terpri)");
  fprintf(fptrbind, "%s\n", " (loop for i from 0 below (gdat-nNumFlowers g) do");
  fprintf(fptrbind, "%s\n", "   (if(<= (abs(- (vec-x (aref (gdat-pfl g) (gdat-curfc g) i)) (vec-x a))) 0.02) ");
  fprintf(fptrbind, "%s\n", "    (if(<= (abs(- (vec-y (aref (gdat-pfl g) (gdat-curfc g) i)) (vec-y a))) 0.02)");
  fprintf(fptrbind, "%s\n", "      (setf (aref (gdat-pvis g) i) 1)");
  fprintf(fptrbind, "%s\n", "    )");
  fprintf(fptrbind, "%s\n", "   )");
  fprintf(fptrbind, "%s\n", " )");
  fprintf(fptrbind, "%s\n", " (make-vec :x 0.0 :y 0.0)");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(defun goy (a)");
  fprintf(fptrbind, "%s\n", " (setf (gdat-ypos g) (+ (gdat-ypos g) (vec-y a)))");
  fprintf(fptrbind, "%s\n", " ;;check if bee is near a flower");
  fprintf(fptrbind, "%s\n", " (terpri)");
  fprintf(fptrbind, "%s\n", " (loop for i from 0 below (gdat-nNumFlowers g) do");
  fprintf(fptrbind, "%s\n", "   (if(<= (abs(- (vec-x (aref (gdat-pfl g) (gdat-curfc g) i)) (vec-x a))) 0.02) ");
  fprintf(fptrbind, "%s\n", "    (if(<= (abs(- (vec-y (aref (gdat-pfl g) (gdat-curfc g) i)) (vec-y a))) 0.02)");
  fprintf(fptrbind, "%s\n", "      (setf (aref (gdat-pvis g) i) 1)");
  fprintf(fptrbind, "%s\n", "    )");
  fprintf(fptrbind, "%s\n", "   )");
  fprintf(fptrbind, "%s\n", " )");
  fprintf(fptrbind, "%s\n", " (make-vec :x 0.0 :y 0.0)");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(defun nf()");
  fprintf(fptrbind, "%s\n", " (aref (gdat-pfl g) (gdat-curfc g) (random 10))");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(defun bee()");
  fprintf(fptrbind, "%s\n", " (make-vec :x (gdat-xpos g) :y (gdat-ypos g)) ;  set value of slots of var ");
  fprintf(fptrbind, "%s\n", ")");

  for ( i = 0; i < run_stats[0].bestn; ++i )
  {
    //pretty_print_individual ( run_stats[0].best[i]->ind, fptrbind);
    int tr = tree_count-1;
    for(; tr >= 0; tr--) // tree
    {
      if(tr != 0)
      {
        //should never get here for single tree, because there are no adfs
	
        fprintf(fptrbind, "(defun %s (a0)", tree_map[tr].name);
        pretty_print_tree ( run_stats[0].best[i]->ind->tr[tr].data, fptrbind);
        fflush(fptrbind); 
      }
      else
      { 
        fprintf(fptrbind, "(defun main ()");
        pretty_print_tree ( run_stats[0].best[i]->ind->tr[tr].data, fptrbind);
        fflush(fptrbind); 
      }
      fprintf(fptrbind, "%s", ")");
      fprintf(fptrbind, "%s", "\n");
    }

    fflush(fptrbind); 
  }

  fprintf(fptrbind, "%s\n", "(loop for fc from 0 below 10 do");
  fprintf(fptrbind, "%s\n", " (setf (gdat-curfc g) fc)");
  fprintf(fptrbind, "%s\n", " (setf (gdat-xpos g) 0.0)");
  fprintf(fptrbind, "%s\n", " (setf (gdat-ypos g) 0.0)");
  fprintf(fptrbind, "%s\n", " (loop for i from 0 below 10 do");
  fprintf(fptrbind, "%s\n", "  (setf (aref (gdat-pvis g) i) 0)");
  fprintf(fptrbind, "%s\n", " )");
  fprintf(fptrbind, "%s\n", " ;;(if(eql fc 3) (progn (print g) (quit) ))");
  fprintf(fptrbind, "%s\n", " ;;(format t \"fitness case ~d: ~s~%\" fc (gdat-pvis g))");
  fprintf(fptrbind, "%s\n", " ;;(print (gdat-pvis g))");
  fprintf(fptrbind, "%s\n", " (main)");
  fprintf(fptrbind, "%s\n", " (format t \"fitness case ~d: ~s~%\" fc (gdat-pvis g))");
  fprintf(fptrbind, "%s\n", " ;;(print (gdat-pvis g))");
  fprintf(fptrbind, "%s\n", ")");
  fprintf(fptrbind, "%s\n", "(quit)");

  fflush(fptrbind); 

  fclose(fptrbind);

  return run_stats[0].best[0]->ind->hits == g.nNumFlowers * APP_FITNESS_CASES;
}

int app_initialize ( int startfromcheckpoint )
{
  int i = 0;
  char *param;

  param = get_parameter ( "app.lawn_width" );
  if ( param == NULL )
    g.lawn_width = APP_DEFAULT_LAWN_WIDTH;
  else
  {
    g.lawn_width = atoi ( param );
    if ( g.lawn_width < 1 )
      error ( E_FATAL_ERROR, "invalid value for \"app.lawn_width\"." );
  }
     
  param = get_parameter ( "app.lawn_height" );
  if ( param == NULL )
    g.lawn_height = APP_DEFAULT_LAWN_HEIGHT;
  else
  {
    g.lawn_height = atoi ( param );
    if ( g.lawn_height < 1 )
      error ( E_FATAL_ERROR, "invalid value for \"app.lawn_height\"." );
  }

  g.nNumFlowers = APP_DEFAULT_NUM_OF_FLOWERS;

  g.pfl = (vector*)MALLOC(g.nNumFlowers * sizeof(vector));
  if(!g.pfl)
    return 1; // out of memory 

  g.pvis = (int*)MALLOC(g.nNumFlowers * sizeof(int));
  if(!g.pvis)
    return 1;

  // assign flower locations based on lawn dimmensions
  // no two flowers can be with-in 0.02 of another flower
  for ( i = 0; i < APP_FITNESS_CASES; ++i )
  {
    for(int f = 0; f < g.nNumFlowers; f++)
    {
      app_fitness_cases[i][f].x = g.lawn_width * random_double();
      app_fitness_cases[i][f].y = g.lawn_height * random_double();
      // move numbers to be centered at 0,0
      app_fitness_cases[i][f].x = app_fitness_cases[i][f].x - (g.lawn_width / 2.0);
      app_fitness_cases[i][f].y = app_fitness_cases[i][f].y - (g.lawn_height / 2.0);
      fvisited[i][f] = 0;

      //check to see if any previous points are the same for this fitness case
      if(f > 0)
      {
        for(int j = f - 1; j >= 0; j--)
        {
          if(fabs(app_fitness_cases[i][f].x - app_fitness_cases[i][j].x) < 0.02)
          {
            //printf("%s\n", "match x");
            if(fabs(app_fitness_cases[i][f].y - app_fitness_cases[i][j].y) < 0.02)
            {
              //printf("%s\n", "match y");
              app_fitness_cases[i][f].x = g.lawn_width * random_double();
              app_fitness_cases[i][f].y = g.lawn_height * random_double();
              // move numbers to be centered at 0,0
              app_fitness_cases[i][f].x = app_fitness_cases[i][f].x - (g.lawn_width / 2.0);
              app_fitness_cases[i][f].y = app_fitness_cases[i][f].y - (g.lawn_height / 2.0);
              j = f;  // restart check
            }
          }
        }
      }


    }
  }

  tstart = time(NULL);

  return 0;
}


/* app_uninitialize()
 *
 * perform application cleanup (free memory, etc.)
 */

void app_uninitialize ( void )
{
  int i,j,k;
  //  

  const char* basefile = get_parameter( "output.basename" );
  char fname[BUFSIZ];
  static FILE* fptrtime=NULL;
  static FILE* fptrdat=NULL;

  strcpy(fname,basefile);
  strcat(fname,".rtime");
  if ((fptrtime=fopen(fname,"w"))==0)
  {
    fprintf(stderr,"Could not open %s to write population.\n",fname);
    exit(1);
  }

  fprintf(fptrtime, "%f", difftime(time(NULL), tstart));
  fclose(fptrtime);

  strcpy(fname,basefile);
  strcat(fname,".dat");
  if ((fptrdat=fopen(fname,"w"))==0)
  {
    fprintf(stderr,"Could not open %s to write dat file.\n",fname);
    exit(1);
  }

  float x,y,z;
  for ( i = 0; i < APP_FITNESS_CASES; ++i )
  {
    for(int f = 0; f < g.nNumFlowers; f++)
    {
      x = app_fitness_cases[i][f].x;
      y = app_fitness_cases[i][f].y;
      fprintf (fptrdat, "%d,%lf,%lf\n", i, x, y);
    }
  }

  fclose(fptrdat);

  FREE( g.pfl );
  FREE( g.pvis );

  return;
}

void app_end_of_breeding ( int gen, multipop *mpop )
{
  return;
}

int app_create_output_streams()
{
  return 0;
}
 
void app_read_checkpoint ( FILE *f )
{
     return;
}

void app_write_checkpoint ( FILE *f )
{
     return;
}

    
