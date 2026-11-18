#include <stdio.h>
#include <stdbool.h>


void checkBalance(float balance);
float deposite();
float withdraw(float balance);


int main()

{

    int choice = 0;
    float balance = 0.0f;
    char run = '\0';
    bool is_running = true;


    printf("\n*****WELCOME TO THE BANK*****\n");


    do {

        printf("\nSelect the option: \n");
        printf("\n1. Check balance: ");
        printf("\n2. Deposite money: ");
        printf("\n3. Withdraw money: ");
        printf("\n4. Exit: \n");
        printf("\n Enter your choice: ");
        scanf("%d", &choice);


        switch (choice)
        {
        case 1:
            checkBalance(balance);
            break;

        case 2:
            balance += deposite();
            break;

        case 3:
            balance -= withdraw(balance);
            break;

        case 4:
            printf("\n Thank you for using the bank!\n");
            break;
        
        default:

            printf("\nInvalid choice!Please select option  1-4\n");
            break;

        }


        }while(choice !=4);
            return 0;
        }


        void checkBalance(float balance)
        {
        
            printf("\nYour current balance is %.2f\n", balance);
            

        }

        float deposite()
        {

            float amount = 0;
            printf("\n Enter amount to deposite: ");
            scanf("%f", &amount);


            if(amount < 0)
            {
                printf("\nInvalid amount\n");
                return 0.0f;


            }

            else
            {

                printf("\nSuccessfully deposited %.2f\n", amount);
                return amount;

            }

        }

        float withdraw(float balance)
        {

            float amount = 0;
            printf("\n Enter amount withdraw: ");
            scanf("%f", &amount);
        

        

        if (amount > balance)
        {
            printf("\nYou have insufficient fund! Your balance is %.2f\n", balance);
            return 0.0f;
        }

            else
            {
                printf("\nSuccessfully withdrawn %.2f\n", amount);
                return amount;

            }
        

    }










        
