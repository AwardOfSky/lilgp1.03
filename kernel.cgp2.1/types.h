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

/*
 gwg Changed data type from int to long for all members in popstat structure.
     For large problems, integer overflow would happen for calculations involving totalnodes. 
     This would affect checkpoint files and all output files involving display of mean. 
 */
/***************** czj: 
  two members added to struct type 'lnode' to implicitly represent 
    the spin wheel for selecting feasible sources accoring to their weights 
  lnode type changed from union to struct
   6/29/96
  lnode type: added an index to handle type checking for each instance of
    a function
*************************/

#ifndef _TYPES_H
#define _TYPES_H

/* holds information about one function (or terminal). */

typedef struct
{
     DATATYPE (*code)();
     void (*ephem_gen)( DATATYPE * );
	 char * (*ephem_str)();
     int arity;
     char *string;
     int type;
     int evaltree;
     int index;
} function;

typedef struct
{
     function *cset;
     int function_count;
     int terminal_count;
     int num_args;
     int size;
} function_set;

/* holds one ERC.  contains the data, reference count, and pointer for
   building a linked list. */

typedef struct _ephem_const
{
     int refcount;
     struct _ephem_const *next;
     DATATYPE d;
     function *f;
} ephem_const;

/* the basic building block of the tree structure.  can be a function pointer,
   a skip value, or a pointer to an ERC. */

typedef struct                                 /*czj - changed from union */
{
     int s;
     function *f;
     ephem_const *d;
     float wheelExt_czj;    /* wheel entry for crossover on extetrnal nodes */
     float wheelInt_czj;                         /* same for internal nodes */
     int typeVec_czj;  /* index to the type vector for this instance of f */
} lnode;

/* one tree -- consists of an array of lnodes.  the size and node counts are
   cached here for speed improvement. */

typedef struct _tree
{
     lnode *data;
     int size;         /* the lnode count */
     int nodes;        /* the actual node count */
} tree;

/* the arguments passed to the function (terminal) code.  can be either a
   value or a pointer to a tree. */

typedef union
{
     DATATYPE d;
     lnode *t;
} farg;

/* one individual.  holds the expression, fitness values, etc. */

typedef struct
{
     tree *tr;
     double r_fitness;
     double s_fitness;
     double a_fitness;
     int hits;
     int evald;
     int flags;
} individual;

/* struct for doing a binary search of successive real-valued intervals. */

typedef struct
{
     double fitness;
     int index;
} reverse_index;

/* one population -- an array of individuals, and some global info. */

typedef struct
{
     individual *ind;
     int size;
     int next;
} population;

typedef int (*select_func_ptr)();
typedef struct _sel_context * (*select_context_func_ptr)();

typedef struct _sel_context
{
     population *p;
     select_func_ptr select_method;
     select_context_func_ptr context_method;
     void *data;
} sel_context;

typedef struct
{
     char *name;
     select_context_func_ptr func;
} select_method;

typedef struct
{
     char *name;
     int (*func)();
} operator;

typedef struct
{
     int copywhole;
     int * from;
     int * as;
     char ** fromsc;
     int to;
     char * tosc;
     int count;
} exchange;

typedef struct
{
     int operator;
     double rate;
     void *data;
     void (*operator_free)();
     void (*operator_start)();
     void (*operator_end)();
     void (*operator_operate)();
} breedphase;

typedef struct
{
     int size, exchanges;
     population **pop;
     exchange *exch;
     breedphase **bpt;
} multipop;

typedef struct
{
     double total;
     reverse_index *ri;
     int count;
} interval_data;

typedef struct
{
     int i;
     ephem_const *e;
} ephem_index;
     
typedef struct _parameter
{
     char *n;
     char *v;
     int copyflags;
} parameter;

typedef struct _saved_ind
{
     individual *ind;
     int refcount;
     struct _saved_ind *next;
} saved_ind;

typedef struct _popstats
{
     /*gwg changed int to long to take care of integer overflow for large problems. */
     long size;
     long maxnodes, minnodes, totalnodes, bestnodes, worstnodes;
     long maxdepth, mindepth, totaldepth, bestdepth, worstdepth;
     long maxhits, minhits, totalhits, besthits, worsthits;
     double bestfit, worstfit, totalfit;
     long bestgen, worstgen;
     long bestpop, worstpop;
     long bestn;
     saved_ind **best;
} popstats;

typedef struct 
{
     lnode *data;
     int size, used;
} genspace;

typedef struct
{
     int fset;
     int nodelimit;
     int depthlimit;
     farg *arguments;
     int argtype;
     int evaluatedfrom;
     char *name;
} treeinfo;



#endif



