#ifndef HORTIFRUIT_H
#define HORTIFRUIT_H

typedef struct {
    char name[50];
    int amount;
    int price;
    char supplier[50];
} Product;

Product *getProducts (int* size);

int getProduct (const char search_name[50], const char search_supplier[50]);
int createProduct (const char name[50], const int price, const char supplier[50]);
int deleteProduct (const char name[50], const char supplier[50]);

int getBalance ();
int setBalance (int balance);

#endif