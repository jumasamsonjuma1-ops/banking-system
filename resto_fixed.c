#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct items
{
    char item[20];
    int qty;
    float price;
};

struct orders
{
    char customer[50];
    char date[12];
    int numOfItems;
    struct items itm[50];
};

void generateBillHeader(char name[50], char date[12]);
void generateBillBody(char item[20], int qty, float price);
void generateBillFooter(float total);

/* This removes the leftover characters after using scanf. */
void clearInputBuffer(void)
{
    int ch;

    while ((ch = getchar()) != '\n' && ch != EOF)
    {
        /* Keep reading until the end of the current input line. */
    }
}

void generateBillHeader(char name[50], char date[12])
{
    printf("\n\n");
    printf("\t   ADV RESTAURANT");
    printf("\n\t   --------------------");
    printf("\nDate: %s", date);
    printf("\nInvoice To: %s", name);
    printf("\n------------------------------\t\t");
    printf("\nItems\t\tQty\t\tTotal");
    printf("\n------------------------------\n");
}

void generateBillBody(char item[20], int qty, float price)
{
    printf("\n%s\t\t%d\t\t%.2f", item, qty, qty * price);
}

void generateBillFooter(float total)
{
    printf("\n------------------------------");
    printf("\nTotal: %.2f", total);
    printf("\n------------------------------\n");
}

int main(void)
{
    int opt, n, InvoiceFound;
    struct orders order;
    char saveBill = 'y';
    char customer[50];
    char contFlag = 'Y';
    FILE *fp;

    /* The loop now checks the actual character entered by the user. */
    while (contFlag == 'Y' || contFlag == 'y')
    {
        system("cls");
        InvoiceFound = 0;

        printf("\t==========ADV RESTAURANT==========");
        printf("\n\n 1. Generate Invoice");
        printf("\n 2. Show All Invoice");
        printf("\n 3. Search Invoice");
        printf("\n 4. Exit");

        printf("\n\n Please select your preferred operation:\t");

        if (scanf("%d", &opt) != 1)
        {
            clearInputBuffer();
            printf("\nInvalid input. Please enter a number.\n");
            continue;
        }
        clearInputBuffer();

        switch (opt)
        {
        case 1:
        {
            float total = 0;
            int validOrder = 1;

            system("cls");
            printf("\nPlease enter the name of the customer:\t");
            fgets(order.customer, 50, stdin);
            order.customer[strcspn(order.customer, "\n")] = '\0';

            strcpy(order.date, __DATE__);

            printf("\nPlease enter the number of items:\t");

            if (scanf("%d", &n) != 1)
            {
                clearInputBuffer();
                printf("\nInvalid number of items.\n");
                break;
            }
            clearInputBuffer();

            /* The array can hold a maximum of 50 items, so reject unsafe values. */
            if (n < 1 || n > 50)
            {
                printf("\nPlease enter a number of items between 1 and 50.\n");
                break;
            }

            order.numOfItems = n;

            for (int i = 0; i < n; i++)
            {
                printf("\n\n");
                printf("Please enter the name of item %d:\t", i + 1);
                fgets(order.itm[i].item, 20, stdin);
                order.itm[i].item[strcspn(order.itm[i].item, "\n")] = '\0';

                printf("Please enter the quantity:\t");

                if (scanf("%d", &order.itm[i].qty) != 1)
                {
                    clearInputBuffer();
                    printf("\nInvalid quantity. Invoice was not generated.\n");
                    validOrder = 0;
                    break;
                }
                clearInputBuffer();

                if (order.itm[i].qty <= 0)
                {
                    printf("\nQuantity must be greater than zero. Invoice was not generated.\n");
                    validOrder = 0;
                    break;
                }

                printf("Please enter the unit price:\t");

                if (scanf("%f", &order.itm[i].price) != 1)
                {
                    clearInputBuffer();
                    printf("\nInvalid price. Invoice was not generated.\n");
                    validOrder = 0;
                    break;
                }
                clearInputBuffer();

                if (order.itm[i].price < 0)
                {
                    printf("\nPrice cannot be negative. Invoice was not generated.\n");
                    validOrder = 0;
                    break;
                }

                total += order.itm[i].qty * order.itm[i].price;
            }

            /* Do not display or save an incomplete invoice after invalid input. */
            if (!validOrder)
            {
                break;
            }

            generateBillHeader(order.customer, order.date);

            for (int i = 0; i < order.numOfItems; i++)
            {
                generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
            }

            generateBillFooter(total);

            printf("\nDo you want to save the invoice [y/n]:\t");

            if (scanf(" %c", &saveBill) != 1)
            {
                clearInputBuffer();
                printf("\nCould not read your choice. Invoice was not saved.\n");
                break;
            }
            clearInputBuffer();

            if (saveBill == 'y' || saveBill == 'Y')
            {
                /* All invoice operations now use the same filename and binary mode. */
                fp = fopen("restaurantBill.dat", "ab");

                if (fp == NULL)
                {
                    printf("\nError: Could not open the invoice file for saving.\n");
                    break;
                }

                /* Check whether fwrite actually saved one complete invoice. */
                if (fwrite(&order, sizeof(struct orders), 1, fp) == 1)
                {
                    printf("\nSuccessfully saved\n");
                }
                else
                {
                    printf("\nError saving invoice.\n");
                }

                fclose(fp);
            }

            break;
        }

        case 2:
        {
            int recordsFound = 0;

            system("cls");

            /* Use the same filename used when invoices are saved. */
            fp = fopen("restaurantBill.dat", "rb");

            if (fp == NULL)
            {
                printf("\nNo saved invoices were found.\n");
                break;
            }

            printf("\n *****Your Previous Invoices***** \n");

            while (fread(&order, sizeof(struct orders), 1, fp) == 1)
            {
                float tot = 0;
                recordsFound = 1;

                generateBillHeader(order.customer, order.date);

                for (int i = 0; i < order.numOfItems; i++)
                {
                    generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                    tot += order.itm[i].qty * order.itm[i].price;
                }

                generateBillFooter(tot);
                printf("\n");
            }

            if (!recordsFound)
            {
                printf("\nNo invoices are currently saved.\n");
            }

            fclose(fp);
            break;
        }

        case 3:
        {
            printf("\nEnter the name of the customer:\t");
            fgets(customer, 50, stdin);

            /* Remove the newline from the name that was actually entered. */
            customer[strcspn(customer, "\n")] = '\0';

            system("cls");
            fp = fopen("restaurantBill.dat", "rb");

            if (fp == NULL)
            {
                printf("\nNo saved invoices were found.\n");
                break;
            }

            printf("\t***** Invoice(s) of %s *****\n", customer);

            while (fread(&order, sizeof(struct orders), 1, fp) == 1)
            {
                float tot = 0;

                /* strcmp returns zero when the two customer names match. */
                if (strcmp(order.customer, customer) == 0)
                {
                    generateBillHeader(order.customer, order.date);

                    for (int i = 0; i < order.numOfItems; i++)
                    {
                        generateBillBody(order.itm[i].item, order.itm[i].qty, order.itm[i].price);
                        tot += order.itm[i].qty * order.itm[i].price;
                    }

                    /* Calculate and display the total for the matching invoice. */
                    generateBillFooter(tot);
                    InvoiceFound = 1;
                }
            }

            if (!InvoiceFound)
            {
                printf("\nSorry, the invoice for %s does not exist.\n", customer);
            }

            fclose(fp);
            break;
        }

        case 4:
            printf("\n====== Thank you for your purchase! Welcome again ======\n\n");
            contFlag = 'N'; /* Stop the main loop when the user chooses Exit. */
            break;

        default:
            printf("\nSorry, invalid option.\n");
            break;
        }

        /* Do not ask the user to continue after selecting Exit. */
        if (contFlag == 'Y' || contFlag == 'y')
        {
            printf("\nDo you want to perform another operation [y/n]: ");

            if (scanf(" %c", &contFlag) != 1)
            {
                contFlag = 'N';
            }

            clearInputBuffer();
        }
    }

    return 0;
}
