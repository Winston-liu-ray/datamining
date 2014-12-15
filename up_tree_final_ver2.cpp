#include<iostream>
//#include <cstdlib>
#include<fstream>

using namespace std ;



int min_util = 0  ;

int num_tr = 0 ;
int num_item = 0 ;

int TR[100][26] ={0}; 
//int mnu[100][26]={0};  //this is for up-growth PLUS

int lock_prof[26] ={0} ;
int hlink_count, find_node_count, local_TD_count ;
char lock_name[26] = {'A','B','C','D','E','F','G','H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 
                       'Q', 'R', 'S', 'T', 'U' ,'V', 'W', 'X', 'Y', 'Z' } ;


int CTU(int T[100][26], int a, int p[26]) ;
struct tree_node * insert_node( struct tree_node *N , char name ,int num) ;
void construct_uptree( struct tree_node *N, int TR[100][26], char name[26], int TU[100], int prof[26]);
void construct_item_hlink(struct tree_node *N, char name );
void construct_tree_hlink(struct tree_node *N, char name[26] );
void table_add(struct table *table_head , int TWU, char name);
struct table * construct_table(int TWU[26], char name[26]);
struct tree_node * find_first_node(struct tree_node *N, char name);
void merge_tree_table(struct tree_node *N, struct table *table);
void up_growth(struct tree_node *N, struct table *table,char name[26],bool PHUI[26]);
void insert_phui(struct phui_link *g_phui, bool n_phui[26], int nu);
void print_phui(struct phui_link *phui);
void identify_hui(struct phui_link *g_phui, int TR[100][26], int prof[26], struct hui_link *g_hui);
void insert_hui(bool phui[26], int nu, struct hui_link *g_hui);
void print_hui(struct hui_link *hui);


struct tree_node
{
		char name ;
	    int nu ;  //utility from the top to it.
		int count ;   
		struct tree_node *parent ;   
		struct tree_node *left ;
		struct tree_node *right ;
		struct tree_node *hlink ;  // link to the same-name node on different branches.

		int mnu ;    // stands for the min number of one item in each transaction of all transactions.
};

struct table
{
	int TWU ;
	char name ;
	struct table *previous ;    // two-ways linking list.
	struct table *next ;
	struct tree_node *hlink ;  // link to up-tree.

};

struct phui_link
{
	bool phui[26];
	int nu ;
	struct phui_link *next ;

};

struct hui_link
{
	int hui[26] ;
	int nu ;
	struct hui_link *next ;

};

struct tree_node *global_root = NULL ;
struct table *global_table = NULL;
struct table *table_current = NULL;
struct tree_node *hlink_current = NULL ;
struct tree_node *hlink_previous = NULL ;	   
struct tree_node *first_node = NULL ;
struct phui_link *global_phui = NULL ;
struct tree_node *temp_node = NULL ;
struct hui_link *global_hui = NULL ;


int main(){


   
   
	
   int i, j, k ;
   int HTWUI_c = 0 ;
   int prof[26] = {0} ;


    ifstream fin("input.txt");

    if(!fin) { 
        cout << "ERROR\n"; 
        system("pause");
        return 0;
    }
 
    fin >> num_item ;    
    fin >> num_tr ;
	
	for(i=0;i<num_tr;i++)
      for(j=0;j<num_item;j++)
        fin >> TR[i][j];       //read transaction number and items

	 fin >> min_util ;  //min sup

     for(j=0;j<num_item;j++)
        fin >> lock_prof[j];  //weight (profit)


	 fin.close();







   int NTR[100][26], TU[100]={0}, TWU[26]={0} ;
   char name[26] ;

   for(i=0;i<num_item;i++)
	   name[i] = lock_name[i] ;   //check which letter it is.

   
   global_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (global_phui == NULL)
                    exit(EXIT_FAILURE) ;

    global_phui->next = NULL ;
    global_phui->nu = 0 ;
    for(i=0;i<num_item;i++)
       global_phui->phui[i] = 0 ;

   global_hui = (struct hui_link *)malloc(sizeof(struct hui_link)) ;
	          if (global_hui == NULL)
                    exit(EXIT_FAILURE) ;

    global_hui->next = NULL ;
    global_hui->nu = 0 ;
    for(i=0;i<num_item;i++)
       global_hui->hui[i] = 0 ;






   for(j=0;j<num_item;j++)
	   prof[j] = lock_prof[j] ;

   for(i=0;i<num_tr;i++)
	   for(j=0;j<num_item;j++)
		   NTR[i][j] = TR[i][j] ;



   for(i=0;i<num_tr;i++)
   {
	   for(j=0;j<num_item;j++)
		   if(TR[i][j]>0)
			   cout << "(" <<name[j] << "," << TR[i][j] << ")  " ;

	   cout << endl ;
   }


   cout << endl ;




   for(i=0;i<num_tr;i++)
   {
     TU[i] = CTU(TR,i, prof) ;   //TU now is the aggregate profit on each transaction.

	 for(j=0;j<num_item;j++)
	    if(TR[i][j]>0)             //TWU will be the total item profit added by transactions which containing specific item  
		 TWU[j]=TWU[j]+TU[i] ;      
   }

   cout << "----------------------------------------------------" << endl ;

   system("pause") ;

    for(i=0;i<num_tr;i++)
	  cout << "T" << i+1 <<  " : " << TU[i] << endl ;
  cout << "----------------------------------------------------" << endl ;

  system("pause") ;

   for(i=0;i<num_item;i++)
	  cout << name[i] << " : " << TWU[i] << endl  ;

   cout << endl ; 

   cout << "----------------------------------------------------" << endl ;

   system("pause") ;


   char ctemp  ;
   int temp ;


   for(i=0;i<num_item-1;i++)      //sort by decreasing order!!!  from now,order has CHANGED.
	   {
		   for(j=0;j<num_item-1-i;j++)
		   {
			   if(TWU[j]<TWU[j+1])
			   { 
				   temp = TWU[j] ;
				   TWU[j] = TWU[j+1] ;
				   TWU[j+1] = temp ;

				   ctemp = name[j]  ;
				   name[j] = name[j+1] ;
				   name[j+1] = ctemp ;

				   temp = prof[j] ;
				   prof[j] = prof[j+1] ;
				   prof[j+1] = temp ;

				   for(k=0;k<num_tr;k++)
				   {
					   temp = NTR[k][j] ;
					   NTR[k][j] = NTR[k][j+1] ;
					   NTR[k][j+1] = temp ;
				   }
			   }
		   }
   }
   

  for(i=0;i<num_item;i++)
	  cout << name[i] << " prof : " << prof[i] 
           << "  TWU : " << TWU[i] << endl  ;      //show the item by TWU order.

   system("pause") ;
   
   cout << endl ;

   cout << "----------------------------------------------------" << endl ;


   for(i=0;i<num_tr;i++)
   {
	   for(j=0;j<num_item;j++)
		   if(NTR[i][j]>0)
			   cout << "(" <<name[j] << "," << NTR[i][j] << ")  " ;      //show the transaction and items in new order.
	   cout << endl ;
   }

	 system("pause") ;
   
	   for(j=0;j<num_item;j++)
		   if(TWU[j] < min_util)
			   {
				   for(i=0;i<num_tr;i++)
				   NTR[i][j] = 0 ;                                    //if below threshold,delete it.
    		   }
	   
      cout << "----------------------------------------------------" << endl ;

    for(i=0;i<num_tr;i++)
       {
	   for(j=0;j<num_item;j++)
		   if(NTR[i][j]>0)
			   cout << "(" <<name[j] << "," << NTR[i][j] << ")  " ;   // show the transaction and item after deleting items.
	      
	   cout << endl ;
       }

	system("pause") ;

	cout << "----------------------------------------------------" << endl ;

	for(i=0;i<num_tr;i++)
     TU[i] = CTU(NTR,i,  prof) ;


	for(i=0;i<num_tr;i++)
	  cout  << "T" << i+1 <<  " : " << TU[i] << endl ;   // After deleting some itemm, show NEW profit.


	system("pause") ;

	cout << "----------------------------------------------------" << endl ;    //Here finishing building up-tree.  Next will be mining.


	
	global_root = (struct tree_node *)malloc(sizeof(struct tree_node)) ;   //construct the root first.
	
	          if (global_root == NULL)    //check root if existing.
                    exit(EXIT_FAILURE) ; 
 global_table = (struct table *)malloc(sizeof(struct table)) ;
	          if (global_table == NULL)
                    exit(EXIT_FAILURE) ; 

	   global_root->name = 'R';
	   global_root->nu = 0 ;
	   global_root->count = 0 ;
	   global_root->parent = NULL  ;
	   global_root->left = NULL ;
	   global_root->right = NULL ;
	   global_root->hlink = NULL ;

	 
	   construct_uptree(global_root, NTR, name, TU, prof) ;    //NTR will represent new transactions and items.
	                                                           // Initiate Up-tree
                                                             //something weird.

     construct_tree_hlink(global_root, name) ;

	 global_table = construct_table(TWU, name) ;    //create table.   TWU stands for total profits of one item.
	

	 merge_tree_table(global_root, global_table) ;   // now root means a tree.  Create a llink from table item connecting to the first node in TREE.


	 bool null_phui[26] = {0} ;

	 up_growth(global_root, global_table, name, null_phui) ; 


	 cout << "---------------------------------------------" << endl ;

	 
	 print_phui(global_phui) ;
	// cout<<"123"<<endl;
	 cout << "---------------------------------------------" << endl ;
	 system("pause") ;

	 identify_hui(global_phui, TR, lock_prof, global_hui);

	 cout << "---------------------------------------------" << endl ;
	 system("pause") ;

	 cout << "high utility itemset :" << endl ;
	 print_hui(global_hui);

    system("pause") ;
	return 0 ;
}    // end of program!

int CTU(int T[100][26], int a, int p[26])
{
	int i,tu=0 ;
	for(i=0;i<num_item;i++)
	{
		tu = tu + T[a][i]*p[i] ;
	}
    return tu ;
}


struct tree_node * insert_node( struct tree_node *N , char name , int num)
{
   struct tree_node *head = NULL ;
   struct tree_node *new_node = NULL ;


   head = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (head == NULL)
                    exit(EXIT_FAILURE) ;

			  
	 head = N->left ;   // N is current node
				  
	if(head==NULL)     // at the beginning ,  add node on the left of root.
	{
        new_node = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (new_node == NULL)
                    exit(EXIT_FAILURE) ;

       new_node->name = name;
	   new_node->nu = 0 ;
	   new_node->count = 1 ;
	   new_node->parent = N  ;   // N is current node
	   new_node->left = NULL ;
	   new_node->right = NULL ;
	   new_node->hlink = NULL ;

	   new_node->mnu = num ;
	//   cout<< new_node->name<<":"<<new_node->mnu<<endl;
	   N->left = new_node ;
	   return new_node ;


	}
	else  
	{
	    if(head->name == name){
		//	cout<<"node1:"<<head->name<<endl;
			head->count = head->count+1 ;
		//	 cout<< head->name;
		//		   cout<<"count:"<<head->count<<endl;

				   
				 if(num<head->mnu)
				 {
                   head->mnu = num ;
                   
				 }
		   
				// cout<< head->name<<":"<<head->mnu<<endl;


		    return head ;
		}
		else
		{
		   while(1)
           {	     
		        if(head->right==NULL)
				{
					new_node = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	                if (new_node == NULL)
                    exit(EXIT_FAILURE) ;
  
                    new_node->name = name;
				//	cout<<"node2:"<<new_node->name<<endl;
        	        new_node->nu = 0 ;
                    new_node->count = 1 ;
                    new_node->parent = N  ;  // N is current node 
                    new_node->left = NULL ;
	                new_node->right = NULL ;
               	    new_node->hlink = NULL ;

					new_node->mnu = num ;
	             //   cout<< new_node->name<<" mnu :"<<new_node->mnu<<endl;

	                head->right = new_node ;
					return new_node ;
					goto success ;


				}
			   
			    if(head->right->name == name)
			      {
				//	  cout<<"node3:"<<head->right->name<<endl;
			       head->right->count = head->right->count+1 ;
				//   cout<< head->right->name;
				//   cout<<"count:"<<head->right->count<<endl;

				   if(num<head->mnu)
				   {
                   head->right->mnu = num ;
	              
				   }
				//    cout<< head->right->name<<":"<<head->right->mnu<<endl;
				   return head->right ;
			        goto success ;
			     }
			   
			    head = head->right ;  //keep searching on the right part.
	
     	   }
		  
		}

	}




	success :  ;

}


void construct_uptree( struct tree_node *N, int TR[100][26], char name[26], int TU[100], int prof[26])
{
	struct tree_node *current = NULL ;
	int i, j, k, increas_nu = 0  ;

	current = (struct tree_node *)malloc(sizeof(struct tree_node)) ;   //initiatial
	          if (current == NULL)
                    exit(EXIT_FAILURE) ; 


	for(i=0;i<num_tr;i++)
	{	
		current = N ;    // N is root.
			//my test
	
	    for(j=0;j<num_item;j++)
		{
		 if(TR[i][j]>0)
		 {  
			current = insert_node(current , name[j],TR[i][j] ) ;
		//	cout<<"current is"<<current->name<<endl;
		
			//cout<<current->name<<endl; 
			increas_nu = TU[i] ;
		     for(k=j+1;k<num_item;++k)
			 {
				 increas_nu = increas_nu - TR[i][k]*prof[k] ;   //calculate by algo.    Sub what's BELOW!!!!
             }
			 current->nu = current->nu + increas_nu ;   //current->nu will be the aggregate nu of different transactions   EX C{D}A + C{D}AB , {D} is added by two transactions.
			 
		 }


		}

       
	}

/*	struct tree_node *test;
	test = (struct tree_node *)malloc(sizeof(struct tree_node)) ; 
	test = N;
	test=test->left->right->left->left->right;
	while(test!=NULL)
	{f
		//cout<<"test:"<<test->name<<endl;
		test=test->right;
	}
*/
	
 
}


void construct_item_hlink(struct tree_node *N, char name )
{
	 if(N->left!=NULL)
	 construct_item_hlink(N->left, name) ;

	 if(N!=NULL)
	 if(N->name == name)
	 {
		 hlink_count++ ;
		 if(hlink_count<=1)
			 hlink_previous=N ;
		 else
		 {
			hlink_previous->hlink=N ;      //connet item from left to right.
			hlink_previous=N ;
		 }
	 }

	 if(N->right!=NULL)
     construct_item_hlink(N->right, name) ;

	 }

void construct_tree_hlink(struct tree_node *N, char name[26] )
{
	int i ;
	for(i=0;i<num_item;i++)
	{
		hlink_count = 0 ;
		construct_item_hlink(N, name[i]);
	}

}	 

void table_add(struct table *table_head , int TWU, char name)
  {
	  struct table *current = NULL ;
      struct table *prev    = NULL ;
	  
	  
	  prev = (struct table *)malloc(sizeof(struct table)) ; 
	    if (prev == NULL)
              exit(EXIT_FAILURE) ;


      current = (struct table *)malloc(sizeof(struct table)) ;
	          if (current == NULL)
                    exit(EXIT_FAILURE) ; 
    
	  prev=table_head ;

	  while(1)
	  {
		   if(prev->next==NULL)
		  {
			  current->next = NULL ;
              current->TWU = TWU ;
			  current->name = name ;
			  current->previous = prev ; 
			  current->next = NULL ;
			  current->hlink = NULL ;
			  prev->next = current ; 
					  break ;
		  }
		   else
		      prev=prev->next ;
	  }

	  
	  prev=NULL ;
 
	  current=NULL ;
  }

struct table * construct_table(int TWU[26], char name[26])
{
	int i ;
	struct table *local_table_output = NULL;
	struct table *local_table_current = NULL;
	struct table *local_table_next = NULL;

	local_table_output = (struct table *)malloc(sizeof(struct table)) ;
	          if (local_table_output == NULL)
                    exit(EXIT_FAILURE) ;	
	local_table_current = (struct table *)malloc(sizeof(struct table)) ;
	          if (local_table_current == NULL)
                    exit(EXIT_FAILURE) ; 
    local_table_next = (struct table *)malloc(sizeof(struct table)) ;
	          if (local_table_next == NULL)
                    exit(EXIT_FAILURE) ; 
    if(TWU[0]>=min_util)
	{                                                //make item A the first(root) 
	  local_table_output->TWU = TWU[0] ;
   	  local_table_output->name = name[0] ;
	  local_table_output->previous = NULL ;
      local_table_output->next = NULL ;
	  local_table_output->hlink = NULL ;
	

	for(i=1;i<num_item;i++)
	{
	  if(TWU[i]>=min_util)                            //only pass the threshold.
		 table_add(local_table_output, TWU[i], name[i]) ;
     }


	}

	return local_table_output ;

}

struct tree_node * find_first_node(struct tree_node *N, char name)   // Using DFS to find the first node (recursive)
{
	       
     
	 if(N->left!=NULL)
	 {
	// 	cout<<"left"<<endl;
	// 	cout<<"test"<<N->name<<endl; 
	 	find_first_node(N->left, name) ;
	 }
	 

    
	 if(N!=NULL)
	 if(N->name == name)
	 {
		 find_node_count++ ;
		 if(find_node_count<=1)
		 {
			 first_node = NULL ;
			 first_node = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (first_node == NULL)
                    exit(EXIT_FAILURE) ;
			 first_node = N ;
			// cout<<"This is :"<<N->name<<endl; 
		 }
	 }

	 if(N->right!=NULL)
	 {
	 //	cout<<"right"<<endl;
	 //	cout<<"test"<<N->name<<endl; 
	 	find_first_node(N->right, name) ;
	 }
     
	// cout<<"passby"<<endl;
	 return first_node ;

}

void merge_tree_table(struct tree_node *N, struct table *table)
{
	 struct table *current = NULL ;

      current = (struct table *)malloc(sizeof(struct table)) ;
	          if (current == NULL)
                    exit(EXIT_FAILURE) ; 

     current = table ;

	  while(1)
	  {
		  find_node_count=0;
		  current->hlink = find_first_node(N, current->name);		  
		  current = current->next ;		
		  if(current == NULL)
			  break ;

	  }

}

void insert_phui(struct phui_link *g_phui, bool n_phui[26], int nu)
{
	
	int i ;
	struct phui_link *current_phui = NULL ;
	struct phui_link *next_phui = NULL ;
	
	current_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (current_phui == NULL)
                    exit(EXIT_FAILURE) ;
	
	
	next_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (next_phui == NULL)
                    exit(EXIT_FAILURE) ;


	if(g_phui->nu == 0)
	{
		g_phui->nu = nu ;
		for(i=0;i<num_item;i++)
		   g_phui->phui[i] = n_phui[i] ;

	}
	else
	{
		current_phui = g_phui ;
		
		while(1)
		{
			if(current_phui->next == NULL)
			{
				next_phui->next = NULL ;
				next_phui->nu = nu ;
			    for(i=0;i<num_item;i++)
                   next_phui->phui[i] = n_phui[i] ;
				current_phui->next = next_phui ;
				
				break ;

			}
			else
			current_phui = current_phui->next ;

		}


	}

}

void up_growth(struct tree_node *N, struct table *table,char name[26],bool PHUI[26])
{
	int i, j, k, local_temp = 0, TU[100] = {0} ;
	int nu_sum = 0 ;
	char local_ctemp ;
	int local_TR[100][26] = {0} ;
	int local_temp_TR[100][26] = {0} ;
	int local_TU[100] = {0} ;
	int local_TWU[26] = {0} ;
	bool local_PHUI[26] = {0} ;
	char local_name[26] ;
	int local_prof[26] = {0} ;

	int local_plus_TR[100][26]={0};
	int local_plus_TU[100]={0};


	int local_mun[100][26]={0};   // for mun

	struct tree_node *local_root = NULL ;

	struct tree_node *current_node = NULL ;

	struct tree_node *temp_current_node = NULL ;

	struct tree_node *path_node = NULL ;

	struct table *local_table = NULL ;
	
	struct table *current_table = NULL ;

	struct phui_link *current_phui = NULL ;

	local_root = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (local_root == NULL)
                    exit(EXIT_FAILURE) ;

	current_node = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (current_node == NULL)
                    exit(EXIT_FAILURE) ; 

	path_node = (struct tree_node *)malloc(sizeof(struct tree_node)) ;
	          if (path_node == NULL)
                    exit(EXIT_FAILURE) ; 
   
	local_table = (struct table *)malloc(sizeof(struct table)) ;
	          if (local_table == NULL)
                    exit(EXIT_FAILURE) ; 

    current_table = (struct table *)malloc(sizeof(struct table)) ;
	          if (current_table == NULL)
                    exit(EXIT_FAILURE) ; 

	current_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (current_phui == NULL)
                    exit(EXIT_FAILURE) ;
	

    
    current_table = table ;
	while(1)
	{    
	//	cout<<"order table is:"<<current_table->name<<endl;
		if(current_table->next ==NULL)
           break ;
		
		   current_table = current_table->next ;   // now it points to {B}
	}

	
	while(1)
	{



    for(i=0;i<num_tr;i++)    //initiate 
	   local_TU[i] = 0 ;
	
	for(i=0;i<num_tr;i++) 
		for(j=0;j<num_item;j++)
			local_TR[i][j] = 0 ;

	for(i=0;i<num_tr;i++)
        local_TU[i] = 0 ;

	for(i=0;i<num_item;i++)
        local_TWU[i] = 0 ;



    for(i=0;i<num_item;i++)       //initiate
		  local_name[i] = lock_name[i] ;   
    for(i=0;i<num_item;i++)
		  local_prof[i] = lock_prof[i] ;            
    for(i=0;i<num_item;i++)
		  local_PHUI[i] = PHUI[i] ;		
		
	
		
		
		
		
		
		
		
		
	  nu_sum = 0 ;
	  current_node = current_table->hlink ;   // point to first B node.


	  
	  local_TD_count = 0 ;

	  while(1)    // this while connecting hlink to all the nodes with same item and aggregate the total nu. 
	  {
	  nu_sum = nu_sum + current_node->nu ;
	
	  if(current_node->parent->name != 'R')
	  {
		  path_node = current_node->parent ;   //trace from down to top, path will begin after the parent of appointed item (here,{B})
		  temp_current_node=current_node->parent;
		  for(i=0;i<num_item;i++)
		  {  
			  if(current_node->name==local_name[i])
		            local_plus_TR[local_TD_count][i]=current_node->mnu;
		  }
		  while(1)
	      {
			  for(i=0;i<num_item;i++)
			  {
			

			    if(path_node->name == lock_name[i])
				{  
					local_plus_TR[local_TD_count][i]=temp_current_node->mnu;
					local_TR[local_TD_count][i] = 1 ;     //本身沒被設進去  ex {B}-CPB   local_TR [B] =0
				//	cout<<"local_plus_TR:"<<local_TD_count<<","<<i<<"is"<<local_plus_TR[local_TD_count][i]<<endl;
					temp_current_node=temp_current_node->parent;
				}
				
			  }
			  local_TU[local_TD_count] = current_node->nu ;

			
	        path_node = path_node->parent ;  // keep tracing up.
	  
	        if(path_node->name == 'R')  //untill root.
			{
				break ;
			}
		        						
	     }

		  local_TD_count ++ ;   //check how many paths.

	  }

	  if(current_node->hlink==NULL)
		  break ;	

	  current_node = current_node->hlink ;
	  }  // end of while
	



	  if(nu_sum>=min_util)
	  for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		     if(local_TR[i][j] == 1)
	            cout << lock_name[j] ;

		  cout << "  TU:  " << local_TU[i] << endl ;  // see the paths above threshold

	  }



	  if(nu_sum>=min_util)
	  {
		  for(i=0;i<num_item;i++)
		    if(current_table->name == lock_name[i] )
			   local_PHUI[i]= 1 ;

      cout << "PHUI :  " ;     
      

	   for(i=0;i<num_item;i++)
	   {
		   if(local_PHUI[i]==1) 
		       cout << lock_name[i] ;
		       
	   }

	   
	   
	   


	   cout << "  {" << nu_sum << "}" ;;
	
	   cout << endl << endl ;

	   insert_phui(global_phui, local_PHUI,nu_sum);

	



	   system("pause") ;
	   
	   cout << "-----------------------------------" << endl ;
	   
	   
       for(i=0;i<num_tr;i++)
       {
	      for(j=0;j<num_item;j++)
	         if(local_TR[i][j]>0)
		        local_TWU[j]=local_TWU[j]+local_TU[i] ;     
        }
	   

	   
	   for(i=0;i<num_item;i++)
		   if(local_TWU[i]>0)
			   cout << local_name[i] <<  " TWU :  " << local_TWU[i] << endl ;
	
	   for(i=0;i<num_item-1;i++)         // sort in decreasing order.  using bubble sort.
	       {
		       for(j=0;j<num_item-1-i;j++)
		       {
			       if(local_TWU[j]<local_TWU[j+1])
			       { 
				       local_temp = local_TWU[j] ;  
				       local_TWU[j] = local_TWU[j+1] ;
				       local_TWU[j+1] = local_temp ;

		    		   local_ctemp = local_name[j];
			    	   local_name[j] = local_name[j+1] ;
	    			   local_name[j+1] = local_ctemp ;
       
	    			   local_temp = local_prof[j] ;
		    		   local_prof[j] = local_prof[j+1] ;
	    			   local_prof[j+1] = local_temp ;


					  
	    			   for(k=0;k<num_tr;k++)
	    			   {
	    				   local_temp = local_TR[k][j] ;
	    				   local_TR[k][j] = local_TR[k][j+1] ;
	    				   local_TR[k][j+1] = local_temp ;
	    			   }
					   for(k=0;k<num_tr;k++)
	    			   {
					   local_temp = local_plus_TR[k][j] ;
		    		   local_plus_TR[k][j] = local_plus_TR[k][j+1] ;
	    			   local_plus_TR[k][j+1] = local_temp ;
	    			   }
		    	   }
		       }
           }	
	
	//   cout << "-----------------------------------" << endl ;

	   for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		     if(local_TR[i][j] == 1)
	            cout << local_name[j] ;


		  cout << "  TU:  " << local_TU[i] << endl ;
		 
	  }

	
	   
	   for(j=0;j<local_TD_count;j++)
		   for(i=0;i<num_tr;i++)
	            local_temp_TR[j][i] = local_TR[j][i] ;
	   

	   
	   for(j=0;j<num_item;j++)
		   if(local_TWU[j]<min_util)    // fliter {A} in first case.
			   {
				   for(i=0;i<num_tr;i++)
				   local_TR[i][j] = 0 ;    // if under min, set 0

    		   }
		
     cout << "-----------------------------------" << endl ;
	
		 
	  for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		     if(local_TR[i][j] == 1)
	            cout << local_name[j] ;

		 // cout<<num_item<<endl;
		  cout << "  TU:  " << local_TU[i] << endl ;
		 
	  }



	  cout << "-----------------------------------" << endl ;

	  // cout<<local_prof[4]<<endl;

	  for(i=0;i<num_item;++i) 
		  if(local_TWU[i]==0)
			  local_prof[i] = 0 ;  
	  
	  for(i=0;i<num_tr;i++)
		  local_plus_TU[i]=local_TU[i];


	  for(i=0;i<local_TD_count;i++)
		  for(j=0;j<num_item;j++)
			  local_TU[i] = local_TU[i] - local_prof[j]*(local_temp_TR[i][j] - local_TR[i][j]) ;  // growth
   

	 //  cout<<local_name[0]<<local_name[1]<<local_name[2]<<local_name[3]<<local_name[4]<<endl;
	  // cout<<"asfasfasf"<<local_plus_TR[3][0]<<local_plus_TR[3][1]<<local_plus_TR[3][2]<<local_plus_TR[3][3]<<local_plus_TR[3][4]<<endl;
	   //  cout<<"asfasfasf"<<local_TR[3][0]<<local_TR[3][1]<<local_TR[3][2]<<local_TR[3][3]<<local_TR[3][4]<<endl;
		// cout<<num_item<<endl;
		

	  for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		  {
			  if(local_TR[i][j]==0)
			  {
			  local_plus_TU[i] = local_plus_TU[i] - local_prof[j]*(local_plus_TR[i][j]) ;  //growth plus
			//  cout<<local_plus_TU[i]<<endl;
			  }
		  }
		  
	  }
	   

	  for(i=0;i<local_TD_count;i++)
		  {
			  k=0 ;
			  for(j=0;j<num_item;++j)
				  k = k + local_TR[i][j] ;
			  if(k == 0)
			  {
				  local_TU[i] = 0 ;
				  local_plus_TU[i]=0;
			  }
	      }



	  for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		     if(local_TR[i][j] == 1)
	            cout << local_name[j] ;


		  cout << "  TU:  " << local_TU[i] << endl ;
		  
	  }

	

	  cout << "-----------------------------------" << endl ;

	  for(i=0;i<local_TD_count;i++)
	  {
		  for(j=0;j<num_item;j++)
		     if(local_TR[i][j] == 1)
	            cout << local_name[j] ;


		 cout<<"   local_plus_TU   "<<" = "<<local_plus_TU[i]<<endl;
		  
	  }
	  
	   cout << "-----------------------------------" << endl ;

	   local_root->name = 'R';
	   local_root->nu = 0 ;
	   local_root->count = 0 ;
	   local_root->parent = NULL  ;
	   local_root->left = NULL ;
	   local_root->right = NULL ;
	   local_root->hlink = NULL ;


	   local_table->hlink = NULL ;
	   local_table->next = NULL ;
	   local_table->previous = NULL ;
	   local_table->TWU = 0 ;


	   k = 0 ;

	  for(i=0;i<local_TD_count;i++)
		 for(j=0;j<num_item;++j)
		     k = k + local_TR[i][j] ;



	  if(k==0)
		  goto endif ;


	   construct_uptree(local_root, local_TR, local_name, local_TU, local_prof) ;

	   construct_tree_hlink(local_root, local_name) ;
	 //  cout<<"create"<<endl;
	   local_table = construct_table(local_TWU, local_name) ;   // this !!!!  important

	   merge_tree_table(local_root, local_table) ;


	   system("pause") ;


	  up_growth(local_root, local_table, local_name, local_PHUI) ; 


	 ;



	   endif :



	   cout << "-----------------------------------" << endl ;
	   cout << "-----------------------------------" << endl ;


	  }
	
	 if(current_table->previous == NULL)
			break;

	  current_table = current_table->previous ;
	//  cout<<current_table->name<<endl;
	//  cout<<"next item"<<endl;
	
	}



}

void print_phui(struct phui_link *phui)
{
	int i ;
	struct phui_link *current_phui = NULL ;
	
	current_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (current_phui == NULL)
                    exit(EXIT_FAILURE) ;

    current_phui = phui ;
	
	if( current_phui->nu!=0)
	while(1)
	{
		cout << "{" ;
		for(i=0;i<num_item;i++)
			if( current_phui->phui[i] == 1)
				cout << lock_name[i] ;

		cout << "} : " << current_phui->nu << endl ;

        
        current_phui = current_phui->next ;
		
		if(current_phui == NULL)
			break ;

	}


}

void insert_hui(bool phui[26], int nu, struct hui_link *g_hui)
{
	int i ;
	struct hui_link *current_hui = NULL ;
	struct hui_link *next_hui = NULL ;
	
	current_hui = (struct hui_link *)malloc(sizeof(struct hui_link)) ;
	          if (current_hui == NULL)
                    exit(EXIT_FAILURE) ;	

	next_hui = (struct hui_link *)malloc(sizeof(struct hui_link)) ;
	          if (next_hui == NULL)
                    exit(EXIT_FAILURE) ;

	if(g_hui->nu == 0)
	{
		g_hui->nu = nu ;
		for(i=0;i<num_item;i++)
		   g_hui->hui[i] = phui[i] ;

	}
	else
	{
		current_hui = g_hui ;
		
		while(1)
		{
			if(current_hui->next == NULL)
			{
				next_hui->next = NULL ;
				next_hui->nu = nu ;
			    for(i=0;i<num_item;i++)
                   next_hui->hui[i] = phui[i] ;
				current_hui->next = next_hui ;
				
				break ;

			}
			else
			current_hui = current_hui->next ;

		}
	}

}


void print_hui(struct hui_link *hui)
{
	int i ;
	struct hui_link *current_hui = NULL ;
	
	current_hui = (struct hui_link *)malloc(sizeof(struct hui_link)) ;
	          if (current_hui == NULL)
                    exit(EXIT_FAILURE) ;

    current_hui = hui ;
	
	if( current_hui->nu!=0)
	while(1)
	{
		cout << "{" ;
		for(i=0;i<num_item;i++)
			if( current_hui->hui[i] == 1)
				cout << lock_name[i] ;

		cout << "} : " << current_hui->nu << endl ;

        
        current_hui = current_hui->next ;
		
		if(current_hui == NULL)
			break ;

	}


}


void identify_hui(struct phui_link *g_phui, int TR[100][26], int prof[26], struct hui_link *g_hui)
{
	int i, j, ui ;
	bool if_exist ;
	struct phui_link *current_phui = NULL ;
	
	current_phui = (struct phui_link *)malloc(sizeof(struct phui_link)) ;
	          if (current_phui == NULL)
                    exit(EXIT_FAILURE) ;	
	
	
    current_phui = g_phui;

	
	if(current_phui->nu!=0)
		while(1)
		{
			ui = 0 ;
			
			for(i=0;i<num_tr;i++)
			{
				if_exist = 1 ;
				for(j=0;j<num_item;j++)
					if(current_phui->phui[j]==1 && TR[i][j]==0)
						if_exist = 0 ;

				if(if_exist==1)
				{
				  for(j=0;j<num_item;j++)
					ui = ui + current_phui->phui[j]*TR[i][j]*prof[j] ;
				
				}


			}

		 for(i=0;i<num_item;i++)
			if( current_phui->phui[i] == 1)
				cout << lock_name[i] ;
		 cout << " : " << ui << endl ; 

		 if(ui>=min_util)
			insert_hui(current_phui->phui, ui, global_hui);




        current_phui = current_phui->next ;
		
		if(current_phui == NULL)
			break ;




		}




}
