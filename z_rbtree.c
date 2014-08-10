#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef  struct  node
{
    int           value;
	bool          color;
	struct node*  parent;
	struct node*  left;
	struct node*  right;
}  node;

void  rb_insert_fixup(node**, node*);
void  rb_delete_fixup(node**, node*);
node* rb_search_node(node*, int value);

void  swap(node* p, node* q)
{
    int temp = p->value;
	p->value = q->value;
	q->value = temp;
}

/* nil结点 */
static node  nil =  {0, false, NULL, NULL, NULL};

/*************************************************************
       x                                  y
	  / \                                / \
     a   y          ----------->        x   c           
	    / \         ----------->       / \
	   b   c                          a   b 
************************************************************/
void  rb_left_rotate(node** root, node* x)
{
    node* y = x->right;
	/* 这里无需检查 因为y最多指向nil 结点所以这里不可能出错 */
	x->right = y->left;
	
	if (y->left != &nil)
	{
	    y->left->parent = x;
	}
	
	y->parent = x->parent;
	/* 如果x的parent是nil则说明x是根结点 此时需要修改根结点的值 */
	if (x->parent == &nil)
	{
	    *root = y;
	}
	else
	{
	    if (x == x->parent->left)
		{
		    x->parent->left = y;
		}
		else
		{
		    x->parent->right = y;
		}
	}
	
	y->left = x;
	x->parent = y;
}

/*******************************************************
           x                     y
		 /  \                  /  \
		y    a   ------>      b    x
	   / \       ------>          / \
	  b   c                      c   a 
********************************************************/
void  rb_right_rotate(node** root, node* x)
{
    node* y = x->left;
	
	x->left = y->right;
	
	if (y->right != &nil)
	{
	    y->right->parent = x;
	}
	
	/* BUGS: 修正BUG 之前忘了加入此语句 */
	y->parent = x->parent;
	if (x->parent == &nil)
	{
	    *root = y;
	}
	else
	{
	    if (x == x->parent->left)
		{
		    x->parent->left = y;
		}
		else
		{
		    x->parent->right = y;
		}
	}
	
	y->right = x;
	x->parent = y;
}

void  rb_insert(node** root, node* z)
{
    node* y = &nil;
	node* x = *root;
	
	while (x != &nil)
	{
	    y = x;
		
		if (x->value == z->value)
		{
		    return;
		}
		else if (x->value > z->value)
		{
		    x = x->left;
		}
		else
		{
		    x = x->right;
		}
	}
	
	z->parent = y;
	
	if (y == &nil)
	{
	    *root = z;
	}
	else
	{
	    if (z->value > y->value)
		{
		    y->right = z;
		}
		else
		{
		    y->left = z;
		}
	}
	
	z->left = &nil;
	z->right = &nil;
	z->color = true;
	
	rb_insert_fixup(root, z);
}

void  rb_insert_fixup(node** root, node* z)
{
    while (z->parent->color == true)
	{
	    /* 因为z->parent的颜色值为红色 所以z->parent不为根 所以这里的z->parent->parent->left一定是有效的 */
	    if (z->parent == z->parent->parent->left)
		{
		    node* y = z->parent->parent->right;
			
			/* 当当前结点的父亲和叔父结点都是红色时 则将父亲结点和叔父结点的颜色都修改为黑色 且将祖父结点修改为红色 此时保证不失去
			   黑色结点的平衡 但是却可能破坏规则4 所以使得z指向z的祖父结点继续进行调整 */
			if (y->color == true)
			{
			    z->parent->color = false;
				y->color = false;
				y->parent->color = true;
				z = y->parent;
			}
			else  /* 当z的叔父结点是黑色的 则此时如果z是其父节点的左孩子则将其父结点着为黑色 其祖父结点着为红色 做右旋 */
			{
			    /* 如果z是其父节点的右结点则先将其父结点做左旋转为与情况3相同的情况 再按情况3的方法处理 */
			    if (z == z->parent->right)
				{
				    z = z->parent;
					rb_left_rotate(root, z);
				}
				
				z->parent->color = false;
				z->parent->parent->color = true;
				rb_right_rotate(root, z->parent->parent);
			}
		}
		else
		{
		    node* y = z->parent->parent->left;
			
			if (y->color == true)
			{
			    z->parent->color = false;
				y->color = false;
				y->parent->color = true;
				z = y->parent;
			}
			else
			{
			    if (z == z->parent->left)
				{
				    z = z->parent;
					rb_right_rotate(root, z);
				}
				
				z->parent->color = false;
				z->parent->parent->color = true;
				rb_left_rotate(root, z->parent->parent);
			}
		}
	}
	
	(*root)->color = false;
}

void  rb_delete(node** root, node* z)
{
    if (z == &nil)
	{
	    return;
	}
	
	node* p        = *root;
	node* real_del = &nil;
	
	/* 找到真正要删除的结点 : 如果要删除的结点最多只有一个孩子 则该结点即真正删除的结点
                              如果要删除的结点有两个孩子 则在其右子树上寻找最左结点 将其记为
                              真正要删除的结点.
	*/
	if (z->left == &nil || z->right == &nil)
	{
	    real_del = z;
	}
	else
	{
	    node* q = z->right;
		node* r = &nil;
		
		/* 找出欲删除结点的右子树中最左边的结点 */
		while (q != &nil)
		{
		    r = q;
            q = q->left;			
		}
		
		real_del = r;
	}
	
	node*  child = &nil;
	
	if (real_del->left == &nil)
	{
	    child = real_del->right;
	}
	else
	{
	    child = real_del->left;
	}
	
	child->parent = real_del->parent;
	
	if (real_del->parent == &nil)
	{
	    *root = child;
	}
	else
	{
		if (real_del == real_del->parent->left)
		{
			real_del->parent->left = child;
		}
		else
		{
			real_del->parent->right = child;
		}
	}
	
	if (real_del != z)
	{
	    swap(real_del, z);
	}
    
	/* 如果删除结点是红色无需调整 因为如果删除的结点是红色则其子结点肯定是黑色 不可能出现破坏规则4的状况 
	   而规则1 2 3更是不可能破坏 因为删除了一个红色结点所以对规则5也没有破坏 因此无需调整
	   
	   所以只有当删除的结点是黑色时才需要调整 
	*/
	if (real_del->color == false)
	{
	    rb_delete_fixup(root, child);
	}
	
	free(real_del);
}

/* 传递过来的结点z是删除结点的孩子结点 */
void   rb_delete_fixup(node** root, node* z)
{   
	/* 如果删除的结点是根结点 则只要保证根结点是黑色即可
	   如果删除结点的孩子结点是红色的则将其置为黑色即可使得性质4符合 且黑高度重新平衡 */
    while (z != *root && z->color == false)
   {
       if (z == z->parent->left)
	   {
	       node*  u = z->parent->right;
           
           if (u->color == true)
           {
		       z->parent->color = true;
			   u->color = false;
			   rb_left_rotate(root, z->parent);
			   u = z->parent->right;
           }
           
		   /* 当前z的兄弟u是黑色 且u的两个孩子都是黑色的 则可以将u置为红色从而使得两兄弟的黑高度相同
		     但是这样整个这部分的黑高度就减少了1 所以将z置为z->parent 开始新的调整 */
           if (u->left->color == false && u->right->color == false)
           {
		        u->color = true;
				z = z->parent;
           }
           else
           {
		       if (u->left->color == true)
			   {
		           u->color = true;
			       u->left->color = false;
			       rb_right_rotate(root, u);
			       u = z->parent->right;
			   }
			   
			   u->color = z->parent->color;
			   z->parent->color = false;
			   u->right->color = false;
			   rb_left_rotate(root, z->parent);
			   z = *root;     /* 将z置为根节点退出循环 */
           }		   
	   }
	   else
	   {
	       node*  u = z->parent->left;
		   
		   if (u->color == true)
		   {
		       z->parent->color = true;
			   u->color = false;
			   rb_right_rotate(root, z->parent);
			   u = z->parent->left;
		   }
		   
		   if (u->left->color == false && u->right->color == false)
		   {
		       u->color = true;
			   z = z->parent;
		   }
		   else
		   {
		       if (u->right->color == true)
			   {
			       u->right->color = false;
				   u->color = true;
				   rb_left_rotate(root, u);
				   u = z->parent->left;
			   }
			   
			   u->color = z->parent->color;
			   z->parent->color = false;
			   u->right->color = false;
			   rb_right_rotate(root, z->parent);
			   z = *root;
		   }
	   }
   }
   
   z->color = false; 
}

node*  rb_search_node(node* root, int value)
{	
	node* p = root;
	
	while (p != &nil)
	{
	    if (p->value == value)
		{
		    return p;
		}
		else if (p->value < value)
		{
		    p = p->right;
		}
		else
		{
		    p = p->left;
		}
	}
	
	return  &nil;
}

node* malloc_node(int value)
{
    node* p = (node*)malloc(sizeof(node));
    
    if (NULL == p)
    {
        return &nil;
    }	
	
	p->value = value;
	p->color = true;
	p->parent = &nil;
	p->left = &nil;
	p->right = &nil;
	
	return  p; 
}

int  main(void)
{
    node*  root = &nil;
    
	
	rb_insert(&root, malloc_node(10));
	rb_insert(&root, malloc_node(20));
	
	rb_delete(&root, rb_search_node(root, 10));
	
	if (rb_search_node(root, 10) != &nil)
	{
	    fprintf(stderr, "find it!\n");
	}
	
	system("pause");
}
