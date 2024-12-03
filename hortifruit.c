#include "file.h"
#include "cJSON.h"
#include "config.h"
#include "hortifruit.h"

#include <stdio.h>

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

Product* getProducts (int* size) {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);
    free (content);

    cJSON *products = cJSON_GetObjectItem (hortifruit, "products");
    *size = cJSON_GetArraySize (products);

    Product* all_products = (Product*)malloc (*size * sizeof (Product));
    if (all_products == NULL) {
        cJSON_Delete (hortifruit);

        return NULL;
    }

    for (int i = 0; i < *size; i++) {
        cJSON *item = cJSON_GetArrayItem (products, i);

        cJSON *name = cJSON_GetObjectItem (item, "name");
        cJSON *amount = cJSON_GetObjectItem (item, "amount");
        cJSON *price = cJSON_GetObjectItem (item, "price");
        cJSON *supplier = cJSON_GetObjectItem (item, "supplier");
        cJSON *expiration = cJSON_GetObjectItem (item, "expiration_date");

        strcpy (all_products[i].name, name -> valuestring);

        all_products[i].amount = amount -> valueint;
        all_products[i].price = price -> valuedouble;

        strcpy (all_products[i].supplier, supplier -> valuestring);
        strcpy (all_products[i].expiration_date, expiration -> valuestring);
    }

    cJSON_Delete (hortifruit);

    return all_products;
}

int getProduct (const char search_name[50], const char search_supplier[50]) {
    char* content = readFile (JSON_PATH);

    int index;
    index = -1;

    cJSON *hortifruit = cJSON_Parse (content);

    if (hortifruit == NULL) {
        cJSON_Delete (hortifruit);

        return index;
    }

    free (content);

    cJSON *products = cJSON_GetObjectItem (hortifruit, "products");

    int size = cJSON_GetArraySize (products);

    for (int i = 0; i < size; i++) {
        cJSON *item = cJSON_GetArrayItem (products, i);

        cJSON *name = cJSON_GetObjectItem (item, "name");
        cJSON *supplier = cJSON_GetObjectItem (item, "supplier");

        if (strcmp (name -> valuestring, search_name) == 0 && strcmp (supplier -> valuestring, search_supplier) == 0) {
            index = i;

            break;
        }
    }

    cJSON_Delete (hortifruit);

    return index;
}

int createProduct (const char name[50], const int amount, const int price, const char supplier[50], char expiration_date[11]) {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);

    if (hortifruit == NULL) {
        cJSON_Delete (hortifruit);

        return 1;
    }

    free (content);

    int index = getProduct (name, supplier);
    cJSON *products = cJSON_GetObjectItem (hortifruit, "products");

    if (index != -1) {
        cJSON *item = cJSON_GetArrayItem (products, index);
        cJSON *amount = cJSON_GetObjectItem (item, "amount");

        int new_amount;
        new_amount = (amount -> valueint + 1);

        cJSON_SetNumberValue (amount, new_amount);
    } else {
        cJSON *new_product = cJSON_CreateObject ();

        cJSON_AddStringToObject (new_product, "name", name);
        cJSON_AddNumberToObject (new_product, "amount", amount);
        cJSON_AddNumberToObject (new_product, "price", price);
        cJSON_AddStringToObject (new_product, "supplier", supplier);
        cJSON_AddStringToObject (new_product, "expiration_date", expiration_date);

        cJSON_AddItemToArray (products, new_product);
    }

    char *new_content = cJSON_Print (hortifruit);
    writeFile (new_content);
    free (new_content);

    cJSON_Delete (hortifruit);

    return 0;
}

int deleteProduct (const char name[50], const char supplier[50]) {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);

    if (hortifruit == NULL) {
        cJSON_Delete (hortifruit);

        return 1;
    }

    free (content);

    int index = getProduct (name, supplier);
    cJSON *products = cJSON_GetObjectItem (hortifruit, "products");

    if (index == -1) {
        cJSON_Delete (hortifruit);

        return 1;
    }

    cJSON_DeleteItemFromArray (products, index);

    char *new_content = cJSON_Print (hortifruit);
    writeFile (new_content);
    free (new_content);

    cJSON_Delete (hortifruit);

    return 0;
}

int setProductAmount (const char name[50], const char supplier[50], const int new_amount) {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);

    if (hortifruit == NULL) {
        cJSON_Delete (hortifruit);

        return 1;
    }

    free (content);

    int index = getProduct (name, supplier);
    cJSON *products = cJSON_GetObjectItem (hortifruit, "products");

    if (index == -1) {
        cJSON_Delete (hortifruit);

        return 1;
    }

    if (new_amount < 1) {
        cJSON_DeleteItemFromArray (products, index);
    } else {
        cJSON *item = cJSON_GetArrayItem (products, index);
        cJSON *amount = cJSON_GetObjectItem (item, "amount");

        cJSON_SetNumberValue (amount, new_amount);
    }

    char *new_content = cJSON_Print (hortifruit);
    writeFile (new_content);
    free (new_content);

    cJSON_Delete (hortifruit);

    return 0;
}

int getBalance () {
    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);
    free (content);

    cJSON *system = cJSON_GetObjectItem (hortifruit, "system");
    cJSON *balance = cJSON_GetObjectItem (system, "balance");

    cJSON_Delete (hortifruit);

    return (int)balance -> valuedouble;
}

int setBalance (int balance) {
    const int actual = getBalance ();

    if (actual == balance) {
        return 1;
    }

    char* content = readFile (JSON_PATH);

    cJSON *hortifruit = cJSON_Parse (content);
    free (content);

    cJSON *system = cJSON_GetObjectItem (hortifruit, "system");
    cJSON *item_balance = cJSON_GetObjectItem (system, "balance");

    cJSON_SetNumberValue (item_balance, balance);

    char *new_content = cJSON_Print (hortifruit);
    writeFile (new_content);
    free (new_content);

    cJSON_Delete (hortifruit);

    return 0;
}