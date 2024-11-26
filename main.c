#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct item
{
    int product_id;
    char product_name[200];
    int product_price;
    int product_count;
    char product_unit[200];
} item;
void addStore()
{
    item product;
    FILE *newItem;

    newItem = fopen("Store.txt", "a");
    if (newItem == NULL)
    {
        perror("Error opening file");
        return 1;
    }

    int product_id;
    int product_price;
    int product_count;
    char product_name[200];
    char product_unit[200];

    printf("Enter the product id := ");
    if (scanf("%d", &product_id) != 1)
    {
        printf("Invalid input for product ID.\n");
        return 1;
    }
    product.product_id = product_id;
    getchar();
    printf("Enter the product name := ");
    if (fgets(product_name, sizeof(product_name), stdin) == NULL)
    {
        printf("Error reading product name.\n");
        return 1;
    }
    product_name[strcspn(product_name, "\n")] = '\0';
    strcpy(product.product_name, product_name);

    printf("Enter the product price := ");
    if (scanf("%d", &product_price) != 1)
    {
        printf("Invalid input for product price.\n");
        return 1;
    }
    product.product_price = product_price;

    printf("Enter the product count := ");
    if (scanf("%d", &product_count) != 1)
    {
        printf("Invalid input for product count.\n");
        return 1;
    }
    product.product_count = product_count;

    getchar();

    printf("Enter the product unit := ");
    if (fgets(product_unit, sizeof(product_unit), stdin) == NULL)
    {
        printf("Error reading product unit.\n");
        return 1;
    }
    product_unit[strcspn(product_unit, "\n")] = '\0';
    strcpy(product.product_unit, product_unit);

    printf("Debugging:\n");
    printf("Product ID: %d\n", product.product_id);
    printf("Product Name: %s\n", product.product_name);
    printf("Product Price: %d\n", product.product_price);
    printf("Product Count: %d\n", product.product_count);
    printf("Product Unit: %s\n", product.product_unit);

    if (fprintf(newItem, "%d %s %d %d %s\n",
                product.product_id,
                product.product_name,
                product.product_price,
                product.product_count,
                product.product_unit) < 0)
    {
        perror("Error writing to file");
        fclose(newItem);
        return 1;
    }

    fclose(newItem);

    printf("Product details have been saved.\n");
}
void deleteStore()
{
    FILE *storeFile, *tempFile;
    item product;
    int removeId, found = 0;

    storeFile = fopen("Store.txt", "r");
    if (storeFile == NULL)
    {
        perror("Error opening Store.txt");
        return;
    }

    tempFile = fopen("TempStore.txt", "w");
    if (tempFile == NULL)
    {
        perror("Error creating TempStore.txt");
        fclose(storeFile);
        return;
    }

    printf("Enter the ID of the product you want to delete: ");
    scanf("%d", &removeId);

    while (fscanf(storeFile, "%d %199s %d %d %199s %d",
                  &product.product_id,
                  product.product_name,
                  &product.product_price,
                  &product.product_count,
                  product.product_unit,
                  &product.product_price) != EOF)
    {
        if (product.product_id == removeId)
        {
            found = 1;
            printf("Product with ID %d (%s) has been deleted successfully.\n",
                   product.product_id, product.product_name);
            continue;
        }
        else
        {
            fprintf(tempFile, "%d %s %d %d %s\n",
                    product.product_id,
                    product.product_name,
                    product.product_price,
                    product.product_count,
                    product.product_unit
                    );
        }
        break;
    }

    if (!found)
    {
        printf("Product with ID %d not found in Store.txt.\n", removeId);
    }

    fclose(storeFile);
    fclose(tempFile);

    remove("Store.txt");
    rename("TempStore.txt", "Store.txt");
}


void addCrate()
{
    FILE *storeFile, *tempFile, *crateFile;
    item product;
    int search_id, quantity, found = 0;

    storeFile = fopen("Store.txt", "r");
    if (storeFile == NULL)
    {
        perror("Error opening Store.txt");
        return;
    }

    tempFile = fopen("TempStore.txt", "a");
    if (tempFile == NULL)
    {
        perror("Error creating TempStore.txt");
        fclose(storeFile);
        return;
    }

    crateFile = fopen("Crate.txt", "a");
    if (crateFile == NULL)
    {
        perror("Error opening Crate.txt");
        fclose(storeFile);
        fclose(tempFile);
        return;
    }

    printf("Enter the product ID: ");
    if (scanf("%d", &search_id) != 1)
    {
        printf("Invalid input for product ID.\n");
        fclose(storeFile);
        fclose(tempFile);
        fclose(crateFile);
        return;
    }

    printf("Enter the quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0)
    {
        printf("Invalid input for quantity.\n");
        fclose(storeFile);
        fclose(tempFile);
        fclose(crateFile);
        return;
    }

    while (fscanf(storeFile, "%d %199s %d %d %199s",
                  &product.product_id,
                  product.product_name,
                  &product.product_price,
                  &product.product_count,
                  product.product_unit) != EOF)
    {
        if (product.product_id == search_id)
        {
            found = 1;

            if (product.product_count >= quantity)
            {
                int bill = quantity * product.product_price;
                fprintf(crateFile, "%d %s %d %d %s %d\n",
                        product.product_id,
                        product.product_name,
                        product.product_price,
                        quantity,
                        product.product_unit,
                        bill);
                printf("Product added to Crate.txt with bill: %d\n", bill);
                product.product_count -= quantity;
            }
            else
            {
                printf("Insufficient quantity available. Only %d left.\n", product.product_count);
            }
        }

        fprintf(tempFile, "%d %s %d %d %s\n",
                product.product_id,
                product.product_name,
                product.product_price,
                product.product_count,
                product.product_unit);
        break;
    }

    if (!found)
    {
        printf("Product with ID %d not found in Store.txt.\n", search_id);
    }

    fclose(storeFile);
    fclose(tempFile);
    fclose(crateFile);

    remove("Store.txt");
    rename("TempStore.txt", "Store.txt");
}

void deleteItem()
{
    FILE *crateFile, *tempFile;
    item product;
    int removeId, quantityToDelete, choice, found = 0;

    crateFile = fopen("Crate.txt", "r");
    if (crateFile == NULL)
    {
        perror("Error opening Crate.txt");
        return;
    }

    tempFile = fopen("TempCrate.txt", "w");
    if (tempFile == NULL)
    {
        perror("Error creating TempCrate.txt");
        fclose(crateFile);
        return;
    }

    printf("Enter the ID of the product you want to delete: ");
    scanf("%d", &removeId);

    while (fscanf(crateFile, "%d %199s %d %d %199s %d",
                  &product.product_id,
                  product.product_name,
                  &product.product_price,
                  &product.product_count,
                  product.product_unit,
                  &product.product_price) != EOF)
    {
        if (product.product_id == removeId)
        {
            found = 1;
            printf("Product found: %s\n", product.product_name);
            printf("1. Delete a specific quantity\n");
            printf("2. Delete the entire product\n");
            printf("Your choice: ");
            scanf("%d", &choice);

            if (choice == 1)
            {
                printf("Enter the quantity to delete: ");
                scanf("%d", &quantityToDelete);
                if (quantityToDelete > 0 && quantityToDelete <= product.product_count)
                {
                    product.product_count -= quantityToDelete;
                    if (product.product_count > 0)
                    {
                        fprintf(tempFile, "%d %s %d %d %s %d\n",
                                product.product_id,
                                product.product_name,
                                product.product_price,
                                product.product_count,
                                product.product_unit,
                                product.product_price);
                    }
                    printf("%d quantity of product removed successfully.\n", quantityToDelete);
                }
                else
                {
                    printf("Invalid quantity to delete. Only %d available.\n", product.product_count);
                    fprintf(tempFile, "%d %s %d %d %s %d\n",
                            product.product_id,
                            product.product_name,
                            product.product_price,
                            product.product_count,
                            product.product_unit,
                            product.product_price);
                }
            }
            else if (choice == 2)
            {
                printf("Product %s removed completely from Crate.txt.\n", product.product_name);
                continue;
            }
            else
            {
                printf("Invalid choice. Keeping product unchanged.\n");
                fprintf(tempFile, "%d %s %d %d %s %d\n",
                        product.product_id,
                        product.product_name,
                        product.product_price,
                        product.product_count,
                        product.product_unit,
                        product.product_price);
            }
        }
        else
        {
            fprintf(tempFile, "%d %s %d %d %s %d\n",
                    product.product_id,
                    product.product_name,
                    product.product_price,
                    product.product_count,
                    product.product_unit,
                    product.product_price);
            break;
        }
    }

    if (!found)
    {
        printf("Product with ID %d not found in Crate.txt.\n", removeId);
    }

    fclose(crateFile);
    fclose(tempFile);

    remove("Crate.txt");
    rename("TempCrate.txt", "Crate.txt");
}

void calculateGrandTotal()
{
    FILE *crateFile;
    item product;
    int bill;
    char product_name[200], product_unit[200];
    int grandTotal = 0;

    crateFile = fopen("Crate.txt", "r");
    if (crateFile == NULL)
    {
        perror("Error opening Crate.txt");
        return;
    }

    while (fscanf(crateFile, "%d %199s %d %d %199s %d",
                  &product.product_id,
                  product.product_name,
                  &product.product_price,
                  &product.product_count,
                  product.product_unit,
                  &bill) != EOF)
    {
        grandTotal += bill;
    }

    fclose(crateFile);

    printf("The grand total of all bills in Crate.txt is: %d\n", grandTotal);
}
void empytCrate()
{
    FILE *crateFile;
    item product;
    int bill;
    crateFile = fopen("Crate.txt", "w");
    if (crateFile == NULL)
    {
        perror("Error opening Crate.txt");
        return;
    }
    while (fscanf(crateFile, "%d %199s %d %d %199s %d",
                  &product.product_id,
                  product.product_name,
                  &product.product_price,
                  &product.product_count,
                  product.product_unit,
                  &bill) != EOF)
    {
        fprintf(crateFile, " ");
    }
    fclose(crateFile);
}

int main()
{
    int user_customer = 100;
    int user_owner = 101;
    int password_customer = 200;
    int password_owner = 202;
    int checkPassword;
    int checkuser;

    printf("Enter the user id := ");
    scanf("%d", &checkuser);
    printf("Enter the password := ");
    scanf("%d", &checkPassword);

    while (1)
    {
        if (checkuser == user_customer && checkPassword == password_customer)
        {
            printf("\n\n\n\n---------------Welcome To The Store----------------\n\n\n\n");
            printf("1.Add To Crate\n2.Delete From Crate\n3.Calculate Total Bill\n4.Exit\n\n\n\n");

            int choice;
            printf("Your choice := ");
            scanf("%d", &choice);

            if (choice == 1)
            {

                addCrate();
                int ch;
            }
            else if (choice == 2)
            {
                deleteItem();
            }
            else if (choice == 3)
            {
                calculateGrandTotal();
            }
            else if (choice == 4)
            {
                printf("Thanks, visit again.\n");
                empytCrate();
                break;
            }
        }
        else if (checkuser == user_owner && checkPassword == password_owner)
        {
            printf("\n\n\n\n---------------Welcome To The Store----------------\n\n\n\n");
            printf("1.Add To Store\n2.Delete From Store\n3.Exit\n\n\n\n");

            int choice;
            printf("Your Choice := ");
            scanf("%d", &choice);

            if (choice == 1)
            {

                addStore();
                int ch;
            }
            else if (choice == 2)
            {
                deleteStore();
            }
            else{
                break;
            }
        }
        else
        {
            printf("Invalid credentials. Exiting.\n");
            break;
        }
    }

    return 0;
}
