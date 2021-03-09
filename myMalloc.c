/** Do documentation properly
	1. README
	2. MAKEFILE
	3. INPUT
	4. OUTPUT
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct alloc
{
	long s,e;
	struct alloc *next,*prev; 
}a;
a *hda, *nna, *tmpa, *hdu, *tmpu, *swap;

void *yalloc(int nb)
{
	swap=(a *)malloc(nb);
	swap->s=0;
	swap->e=0;
	swap->prev=NULL;
	swap->next=NULL;
	hdu=swap+1;
	printf("\n %p %p \n",swap,hdu);
	hdu->s=(long)hdu+sizeof(a);
	hdu->e=hdu->s+nb-1-2*sizeof(a);	
	hdu->prev=NULL;
	hdu->next=NULL;
	hda=NULL;
	return (void *)hdu->s;	
}

void disp()
{	
	tmpa=hda;
	tmpu=hdu;
	printf("\n alloc : ");
	if(hda!=NULL){	
		while(tmpa->next!=NULL)
		{
			printf(" (%ld,%ld) ->",tmpa->s,tmpa->e);
			tmpa=tmpa->next;
		}
		printf(" (%ld,%ld) ->",tmpa->s,tmpa->e);
	}
	else
		printf("NULL");
		
	printf("\n unalloc : ");
	while(tmpu->next!=NULL)
	{
		printf(" (%ld,%ld) ->",tmpu->s,tmpu->e);
		tmpu=tmpu->next;
	}
	printf(" (%ld,%ld) ->",tmpu->s,tmpu->e);
}

void *ymalloc(int nbt)		
{
	long nb=(long)nbt;
	tmpu=hdu;
	if(nb<1)
		return NULL;
	while(1)
		if(tmpu->e-tmpu->s+1-sizeof(a)>=nb)
			break;
		else
		{
			tmpu=tmpu->next;
			if(tmpu==NULL)
			{
				printf("\n NOT ENOUGH MEM.. \n");
				return NULL;
			}
		}
	if(hda==NULL)		
	{
		hda=hdu;
		swap->e=hdu->e;	
		hda->e=hda->s+nb-1;
		tmpu=(a *)(hda->e+1);
		tmpu->s=(long)tmpu+sizeof(a);
		tmpu->e=swap->e;
		hdu=tmpu;
	}
	else
	{
		tmpa=hda;
		while(tmpa->next!=NULL)
			tmpa=tmpa->next;
		nna=tmpu;
		swap->e=nna->e;
		swap->prev=nna->prev;
		swap->next=nna->next;
		nna->e=nna->s+nb-1;
		nna->next=NULL;
		nna->prev=tmpa;
		tmpa->next=nna;
		tmpu=(a *)(nna->e+1);
		tmpu->s=(long)tmpu+sizeof(a);
		tmpu->e=swap->e;
		tmpu->prev=swap->prev;
		tmpu->next=swap->next;
		if(nna==hdu)
			hdu=tmpu;
	}
	if(tmpu->e-tmpu->s==-1)	
	{
		if(tmpu==hdu && hdu->next==NULL);
		else
		if(tmpu==hdu && hdu->next!=NULL)  
		{
			hdu=hdu->next;
			hdu->prev=NULL;
		}
		else
		if(tmpu->next==NULL && tmpu!=hdu)
		{
			tmpu=tmpu->prev;
			tmpu->next=NULL; 
		}
		else
		{
			tmpu->next->prev=tmpu->prev;
			tmpu->prev->next=tmpu->next;
		}
	}
	if(hda->next==NULL)
		return (void *)hda->s;
	else
		return (void *)nna->s;
}

void yfree(void *ptr) 
{
	long p=(long)ptr;
	tmpa=hda;
	while(tmpa->s!=p)
		if((tmpa=tmpa->next)==NULL)
			break;
	if(tmpa==NULL)
	{
		printf("\n %ld ptr NOT FOUND",p);
		return;
	}

	//1] head alloc
	if(tmpa==hda)
	{
		// break from alloc
		if(hda->next==NULL)
		{
			hda=NULL;
		}
		else
		{
			hda=hda->next;
			hda->prev=NULL;
		}
		//connect to unalloc

		//headu-1
		if(tmpa->e+1+sizeof(a)==hdu->s)
		{
			hdu->s=tmpa->s; 
			return;
		}

		//headu-2
		if(tmpa->e+1+sizeof(a)<hdu->s)
		{
			tmpa->next=hdu;
			hdu->prev=tmpa;
			hdu=tmpa;
			hdu->prev=NULL;
			return;
		}

		//middle
		a *ftu,*stu;	//ftu - first traverse unalloc
		ftu=hdu;	//stu - second traverse unalloc
		stu=hdu->next;
		while(ftu->next!=NULL) //check out - considering atleast 3 nodes
		{
			//mid-1
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				ftu->next=tmpa;
				tmpa->prev=ftu;
				tmpa->next=stu;
				stu->prev=tmpa; 
				return;
			}

			//mid-2
			if(ftu->e+1+sizeof(a)==tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=tmpa->e; 
					return;
				}
				else
				{
					ftu->e=tmpa->e; 
					return;
				}
			}

			//mid-3
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)==stu->s)
			{
				ftu->next=tmpa;
				tmpa->s=stu->s;
				tmpa->prev=ftu;
				return;
			}

			//mid-4
			if(ftu->e+1==tmpa->s && tmpa->e+1==stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=stu->e;
					hdu->next=stu->next; 
					return;
				}
				else
				{
					ftu->e=stu->e;
					ftu->next=stu->next; 
					return;
				}
			}
			ftu=stu;
			stu=stu->next;
		}//while end

		//end ualloc
		if(ftu->e+1==tmpa->s)
		{
			ftu->e=tmpa->e; 
			return;
		}
		else
		{
			ftu->next=tmpa;
			tmpa->prev=ftu;
			tmpa->next=NULL; 
			return;
		}
	}// end if of head alloc

	//2] end alloc
	if(tmpa->next==NULL)
	{
		//break;
		tmpa->prev->next=NULL;

		//connect to unalloc

		//headu-1
		if(tmpa->e+1+sizeof(a)==hdu->s)
		{
			hdu->s=tmpa->s; 
			return;
		}

		//headu-2
		if(tmpa->e+1+sizeof(a)<hdu->s)
		{
			tmpa->next=hdu;
			hdu->prev=tmpa;
			hdu=tmpa;
			hdu->prev=NULL;
			return;
		}

		//middle
		a *ftu,*stu;	//ftu - first traverse unalloc
		ftu=hdu;	//stu - second traverse unalloc
		stu=hdu->next;
		while(ftu->next!=NULL) //check out - considering atleast 3 nodes
		{
			//mid-1
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				ftu->next=tmpa;
				tmpa->prev=ftu;
				tmpa->next=stu;
				stu->prev=tmpa; 
				return;
			}

			//mid-2
			if(ftu->e+1+sizeof(a)==tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=tmpa->e; 
					return;
				}
				else
				{
					ftu->e=tmpa->e; 
					return;
				}
			}

			//mid-3
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)==stu->s)
			{
				ftu->next=tmpa;
				tmpa->s=stu->s;
				tmpa->prev=ftu;
				return;
			}

			//mid-4
			if(ftu->e+1==tmpa->s && tmpa->e+1==stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=stu->e;
					hdu->next=stu->next; 
					return;
				}
				else
				{
					ftu->e=stu->e;
					ftu->next=stu->next; 
					return;
				}
			}
			ftu=stu;
			stu=stu->next;
		}//while end

		//end ualloc
		if(ftu->e+1==tmpa->s)
		{
			ftu->e=tmpa->e; 
			return;
		}
		else
		{
			ftu->next=tmpa;
			tmpa->prev=ftu;
			tmpa->next=NULL; 
			return;
		}
	}// end if of end alloc

	//3]middle of alloc
	else
	{
		//break
		tmpa->prev->next=tmpa->next;
		tmpa->next->prev=tmpa->prev;

		//connect to unalloc
	
		//headu-1
		if(tmpa->e+1+sizeof(a)==hdu->s)
		{
			hdu->s=tmpa->s; 
			return;
		}

		//headu-2
		if(tmpa->e+1+sizeof(a)<hdu->s)
		{
			tmpa->next=hdu;
			hdu->prev=tmpa;
			hdu=tmpa;
			hdu->prev=NULL;
			return;
		}

		//middle
		a *ftu,*stu;	//ftu - first traverse unalloc
		ftu=hdu;	//stu - second traverse unalloc
		stu=hdu->next;
		while(ftu->next!=NULL) //check out - considering atleast 3 nodes
		{
			//mid-1
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				ftu->next=tmpa;
				tmpa->prev=ftu;
				tmpa->next=stu;
				stu->prev=tmpa; 
				return;
			}

			//mid-2
			if(ftu->e+1+sizeof(a)==tmpa->s && tmpa->e+1+sizeof(a)<stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=tmpa->e; 
					return;
				}
				else
				{
					ftu->e=tmpa->e; 
					return;
				}
			}

			//mid-3
			if(ftu->e+1+sizeof(a)<tmpa->s && tmpa->e+1+sizeof(a)==stu->s)
			{
				ftu->next=tmpa;
				tmpa->s=stu->s;
				tmpa->prev=ftu;
				return;
			}

			//mid-4
			if(ftu->e+1==tmpa->s && tmpa->e+1==stu->s)
			{
				if(ftu==hdu)
				{
					hdu->e=stu->e;
					hdu->next=stu->next; 
					return;
				}
				else
				{
					ftu->e=stu->e;
					ftu->next=stu->next; 
					return;
				}
			}
			ftu=stu;
			stu=stu->next;
		}//while end

		//end ualloc
		if(ftu->e+1==tmpa->s)
		{
			ftu->e=tmpa->e; 
			return;
		}
		else
		{
			ftu->next=tmpa;
			tmpa->prev=ftu;
			tmpa->next=NULL; 
			return;
		}
	}//end middle of alloc
}//yfree end
