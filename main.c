#include <stdio.h>
#include <stdlib.h>

struct queue {
	char sig;
	int bt;
	int pri;
	struct queue *n;
};

void queue_add(struct queue **q, char sig, int bt, int pri)
{
	struct queue *t, **it;

	t = malloc(sizeof *t);
	t->sig = sig;
	t->bt = bt;
	t->pri = pri;

	for(it = q; *it != NULL && (*it)->pri <= pri; it = &(*it)->n);

	t->n = *it;
	*it = t;
}

struct queue *queue_rm(struct queue **q)
{
	struct queue *r;

	r = *q;
	*q = (*q)->n;
	
	return r;
}

int main()
{
	struct queue *h, *m, *l, *cur;
	int done;
	int op;
	int bt;
	int pri;
	char sig;
	int tot_el;
	int rr;
	int cur_q;
	int el;

	h = NULL;
	m = NULL;
	l = NULL;

	done = 0;
	tot_el = 0;
	rr = 4;
	el = 0;

	while(!done) {
		printf("1. High\n2. Mid\n3. Low\n4. End\nEnter Value: ");
		scanf("%d", &op);

		if(op == 4) {
			done = 1;
			break;
		}
		
		printf("Enter single char signifier: ");
		while((sig = fgetc(stdin)) == '\n');
		printf("Enter burst time: ");
		scanf("%d", &bt);
		
		switch(op) {
		case 1:
			queue_add(&h, sig, bt, 0);
			break;
		case 2:
			printf("Enter priority of this process: ");
			scanf("%d", &pri);
			queue_add(&m, sig, bt, pri);
			break;
		case 3:
			queue_add(&l, sig, bt, 0);
			break;
		default:
			printf("Invalid option\n");
			break;
		}
	}

	cur_q = 1;
	while(h != NULL || m != NULL || l != NULL) {
		if(cur_q == 1 && h == NULL) {
			cur_q = 2;
			tot_el = 0;
		}
		if(cur_q == 2 && m == NULL) {
			cur_q = 3;
			tot_el = 0;
		}
		if(cur_q == 3 && l == NULL) {
			cur_q = 1;
			tot_el = 0;
		}

		if(cur_q == 1) {
			cur = queue_rm(&h);
			printf("'%c' is exec\n", cur->sig);
			if(cur->bt <= rr) {
				el = cur->bt;
				rr -= el;
				free(cur);
			}
			else {
				el = rr;
				rr = 0;
				cur->bt -= el;
				queue_add(&h, cur->sig, cur->bt, cur->pri);
				free(cur);
			}

			tot_el += el;

			if(tot_el < 10) {
				if(10 - tot_el >= 4)
					rr = 4;
				else
					rr = 10 - tot_el;
			}
			else {
				cur_q = 2;
				tot_el = 0;
			}
		}
		else if(cur_q == 2) {
			cur = queue_rm(&m);
			printf("'%c' is exec\n", cur->sig);
			if(tot_el + cur->bt < 10) {
				el = cur->bt;
				free(cur);
			}
			else {
				el = 10 - tot_el;
				queue_add(&m, cur->sig, cur->bt, cur->pri);
				free(cur);
			}

			tot_el += el;

			if(tot_el == 10) {
				cur_q = 3;
				tot_el = 0;
			}
		}
		else if(cur_q == 3) {
			cur = queue_rm(&l);
			printf("'%c' is exec\n", cur->sig);
			if(tot_el + cur->bt < 10) {
				el = cur->bt;
				free(cur);
			}
			else {
				el = 10 - tot_el;
				queue_add(&l, cur->sig, cur->bt, cur->pri);
				free(cur);
			}

			tot_el += el;

			if(tot_el == 10) {
				cur_q = 1;
				tot_el = 0;
			}
		}
	}

	return 0;
}
