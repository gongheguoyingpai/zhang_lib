/*
  Search Tree : This head file is used to build search tree. Use this file
  you can insert new node into a search tree, and delete a node from a search
  tree, and you can find out the max num and the min num in a tree.
  
  You won't use malloc() and free() to manage your memory, but only use the
  functions we provide to operat the tree which you create. Use create_searchtree()
  to create a new tree, and at last use destroy_searchtree() to destroy it.
  The memory will be managed well.
  
  It is supposed to work well, but we don't provide any warranty.
  
  <ul>
   <li>author :  zhang</li>
   <li>email  :  gongheguoyingpai@163.com</li>
   </ul>
 */
#ifndef   Z_SEARCHTREE_H
#define   Z_SEARCHTREE_H

#include <stdbool.h>

typedef struct TreeNode
{
    int        value;
    struct TreeNode*  leftchild;
    struct TreeNode*  rightchild;
    struct TreeNode*  parent;
} TreeNode;

typedef  struct Tree
{
    TreeNode*  root;
    int        size;
}* Tree;

enum ORDERKIND {PRE, IN, POST};


static Tree create_searchtree()
{
    Tree  tree = (Tree)malloc(sizeof(struct Tree));
    
    tree->root = NULL;
    
    return  tree;
}

static bool searchtree_insert_node(Tree tree, int value)
{
    TreeNode*  node = (TreeNode*)malloc(sizeof(TreeNode));
    
    node->value = value;
    node->leftchild = NULL;
    node->rightchild = NULL;
    
    if (tree->root == NULL)
    {
        tree->root = node;
        node->parent = NULL;
    }
    else
    {
        TreeNode* temp = tree->root;
        
        while (temp != NULL)
        {
            if (node->value > temp->value)
            {
                if (temp->rightchild == NULL)
                {
                    temp->rightchild = node;
                    node->parent = temp;
                    ++tree->size;
                    return true;
                }
                else
                {
                    temp = temp->rightchild;
                }
            }
            else if (node->value < temp->value)
            {
                if (temp->leftchild == NULL)
                {
                    temp->leftchild = node;
                    node->parent = temp;
                    ++tree->size;
                    return true;
                }
                else
                {
                    temp = temp->leftchild;
                }
            }
            else
            {
                return false;
            }
        }
    }
}

static void  preorder_traverse(TreeNode* root)
{
    if (root != NULL)
    {
        printf("%d\n", root->value);
        preorder_traverse(root->leftchild);
        preorder_traverse(root->rightchild);
    }
}

static void  inorder_traverse(TreeNode* root)
{
    if (root != NULL)
    {
        inorder_traverse(root->leftchild);
        printf("%d\n", root->value);
        inorder_traverse(root->rightchild);
    }
}

static void  postorder_traverse(TreeNode* root)
{
    if (root != NULL)
    {
         postorder_traverse(root->leftchild);
         postorder_traverse(root->rightchild);
         printf("%d\n", root->value);
    }
}

static void  output_tree(Tree tree, enum ORDERKIND kind)
{
    TreeNode*  root = tree->root;
    
    switch(kind)
    {
        case  PRE:
              preorder_traverse(root);
              break;
        
        case  IN:
              inorder_traverse(root);
              break;
              
        case  POST:
              postorder_traverse(root);
              break;
    }
}

static int searchtree_minimum(Tree tree)
{
    TreeNode* root = tree->root;
    
    while ((root != NULL) && (root->leftchild != NULL))
    {
        root = root->leftchild;
    }
    
    return  root->value;
}

static int searchtree_maximum(Tree tree)
{
    TreeNode* root = tree->root;
    
    while ((root != NULL) && (root->rightchild != NULL))
    {
        root = root->rightchild;
    }
    
    return  root->value;
}

static TreeNode* searchtree_search(Tree tree, int value)
{
    TreeNode*  root = tree->root;
    
    while ((root != NULL) && (root->value != value))
    {
        root = root->value > value ? root->leftchild : root->rightchild;
    }
    
    return  root;
}

static bool  searchtree_delete_node(Tree tree, int value)
{
    TreeNode*  pos = searchtree_search(tree, value);
    TreeNode*  del_pos = NULL;
    
    if (pos == NULL)
    {
        return  false;
    }
    
    /* --------Find out which node will be deleted. -------- */
    
    /* If the node that you want to delete has no leftchild or/and rightchild.
       This node will be delete.*/
    if (pos->leftchild == NULL || pos->rightchild == NULL)
    {
        del_pos = pos;
    } 
    else /* If the node that you want to delete has leftchild and rightchild, the delete node will be the node's rightchild-tree's smallest node */
    {
        TreeNode*  temp = pos->rightchild;
        
        while ((temp != NULL) && (temp->leftchild != NULL))
        {
            temp = temp->leftchild;
        }
        
        del_pos = temp;
    }
    
    /* ---------------------------------------------------- */
    
    /* ---Find out del_pos's which child will be linked to its parent.--- */
    TreeNode*  temp = NULL;
    
    if (del_pos->leftchild != NULL)
    {
        temp = del_pos->leftchild;
    }
    else
    {
        temp = del_pos->rightchild;
    }
    /* ---------------------------------------------------- */
    
    /* ---If del_pos's one child is not NULL, we will change its parent point.--- */
    if (temp != NULL)
    {
        temp->parent = del_pos->parent;
    }
    /* -------------------------------------------------------------------------- */
    
    /* ---Make the del_pos leave tree and make the del_pos's parent point to the right node--- */
    if (del_pos->parent == NULL)
    {
        tree->root = temp;
    }
    else if (del_pos == del_pos->parent->leftchild)
    {
        del_pos->parent->leftchild = temp;
    }
    else
    {
        del_pos->parent->rightchild = temp;
    }
    /* ---------------------------------------------------------------------------------------- */
    
    /* ---If the deleted node is not which we want to delete but is that we want to use to instead
       which we want to delete, we must change two notes' value, it make it look like, we delete 
       which we want to delete.---*/
    if (del_pos != pos)
    {
        pos->value = del_pos->value;
    }
    
    free(del_pos);
    --tree->size;
}

static int  searchtree_nodes(Tree tree)
{
     return tree->size;
}

static void z_searchtree_destroy(TreeNode* root)
{
    if (root != NULL)
    {
        z_searchtree_destroy(root->leftchild);
        z_searchtree_destroy(root->rightchild);
        free(root);
    }
}

static void destroy_searchtree(Tree tree)
{
    TreeNode*  temp = tree->root;
    
    if (temp != NULL)
    {
        z_searchtree_destroy(temp);
    }
    
    free(tree);
    tree = NULL;
}

#endif
