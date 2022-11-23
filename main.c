#include"prototype.h"

void struct_write_file()//when finish one operation, overwrite file with new data in struct
{
    FILE * donation = fopen("donation.txt", "w");//open file in write mode
    fprintf(donation, "%-23s : %-13s : %-13s : %-15s : %-30s\n",
    data_name[0], data_name[1], data_name[2], data_name[3], data_name[4]);//print the column name of data into file first
    for(int i = 0; i < 5; i++)
    {
        fprintf(donation, "%-23s : %-13s : %-13s : %-15d : %-30.4f\n", 
        initial_info[i].name, initial_info[i].code, initial_info[i].donator,
        initial_info[i].shipment_no, initial_info[i].quantity);
    }//print the data into the file
    printf("Finish txt file updating.\n");//inform user that the data is successful updated
    fclose(donation);//close the file opened
}

void read_txt_quantity()//read the txt quantity from text file
{
    FILE * fp = fopen("donation.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    int count = 0;
    fgets(line, 255, fp);//gets the data name line in text file
    while(fgets(line, 255, fp) != NULL)
    {
        double quantity = get_paste_line_quantity(line, count);
        count++;
    }//get the quantity number from each line and update it in system
    printf("Quantity Received read & paste.\n");////inform user that the data is successful updated
    fclose(fp);//close the file opened
}

void read_txt_shipment()//read shipment number from file
{
    FILE * fp = fopen("donation.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    int count = 0;
    fgets(line, 255, fp);
    while(fgets(line, 255, fp) != NULL)
    {
        int shipment = get_paste_line_shipment(line, count);
        count++;
    }//get the shipment number from each line and update it in system
    printf("Shipment number read & paste.\n");//inform user that the data is successful updated
    fclose(fp);//close the file opened
}

void read_txt_code()//read supply code from file
{
    FILE * fp = fopen("donation.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    int count = 0;
    fgets(line, 255, fp);
    char code[3] = {0};
    while(fgets(line, 255, fp) != NULL)
    {
        get_paste_line_code(line, count);
        count++;
    }//get the supply code from each line and update it in system
    printf("Supply code read & paste.\n");//inform user that the data is successful updated
    fclose(fp);//close the file opened
}

void read_txt_donator()//read donator name from file
{
    FILE * fp = fopen("donation.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    int count = 0;
    fgets(line, 255, fp);
    while(fgets(line, 255, fp) != NULL)
    {
        get_paste_line_donator(line, count);
        count++;
    }//get the donator name from each line and update it in system
    printf("Donator read & paste.\n");//inform user that the data is successful updated
    fclose(fp);//close the file opened
}

void read_txt_name()//read name of supply from file
{
    FILE * fp = fopen("donation.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    int count = 0;
    fgets(line, 255, fp);
    while(fgets(line, 255, fp) != NULL)
    {
        get_paste_line_name(line, count);
        count++;
    }//get the name of supply from each line and update it in system
    printf("Name read & paste.\n");//inform user that the data is successful updated
    fclose(fp);//close the file opened
}

void read_txt_file()//read data from file
{
    read_txt_name();
    read_txt_donator();
    read_txt_code();
    read_txt_shipment();
    read_txt_quantity();
}// call all the function related to extract data from text file

void donation_input()//get donation received data
{
    int code_index = supply_code_input();//ask user to enter supply code
    int shipment = initial_info[code_index].shipment_no + 1;//add the number of shipment first
    printf("Please enter quantity received of donation (in millions): ");//ask use to enter quantity received in million
    fflush(stdin);//clear the buffer
    float quantity = 0;
    scanf("%f", &quantity);
    fflush(stdin);
    if(quantity <= 0)
    {
        printf("Invalid donation quantity.Please reenter.\n");//recall the function for invalid input
        donation_input();
        return ;//stop function
    }
    quantity_modify_confirm(shipment, quantity, code_index);//ask user to make confirmation
}

void stock_dist(int code_index, float quantity, char hospital[200])//process distribution data
{
    if (quantity > initial_info[code_index].quantity)
    {
        printf("No enough stock for distribution.\n");//tell user that the enter stock is not enough for distribution
        int choice;
        while(1)
        {
            printf("Back to main menu\tPress 1\nReenter quantity\tPress 2\n");//ask user whether back to main menu or reenter data
            printf("Please enter your choice: ");
            scanf("%d", &choice);
            if(choice == 1)
            {
                menu();
            }
            else if(choice == 2)
            {
                printf("Please enter quantity to be distributed (in thousands): ");
                scanf("%f", &quantity);
                quantity /= 1000;//change the number to million
                stock_dist(code_index, quantity, hospital);
            }
            else
            {
                printf("Invalid choice.\n");//ask again when invalid input
                fflush(stdin);
                continue;
            }
            break;
        }
    }
    else
    {
        quantity_dist_confirm(quantity, code_index, hospital);//send distribution confirmation to user
    }
}

void distribution()//start distribution process
{
    int code_index = supply_code_input();//ask user for the supply code
    char hospital[200] = {0};
    while(1)
    {
        int sentinel = 0;
        printf("Please enter the hospital name ['|' symbol is prohibited]:");//ask user to enter receiving hospital name
        fgets(hospital, 200, stdin);
        for(int i = 0; i < strlen(hospital);i++)
        {
            if(hospital[i] == '|')
            {
                printf("Invalid hospital name.Please reenter hospital name.\n");//ask user to enter valid hospital name
                sentinel = 1;
            }
        }
        if(sentinel == 0)
            break;
    }
    hospital[strlen(hospital) - 1] = '\0';
    fflush(stdin);
    printf("Please enter quantity to be distributed (in thousands): ");//ask user to enter distributed quantity
    fflush(stdin);
    float quantity = 0;
    scanf("%f", &quantity);
    fflush(stdin);
    if(quantity <= 0)
    {
        printf("Invalid distribution quantity.Please reenter.\n");//ask user to reenter valid distribution quantity
        distribution();
        return ;//stop function
    }
    quantity /= 1000;
    stock_dist(code_index, quantity, hospital);//process distribution data
}

void quantity_check()//check the donation quantity based on supply code
{
    int code_index = supply_code_input();//ask user to enter supply code
    printf("\n%-30s: %s\n%-30s: %s\n%-30s: %s\n%-30s: %d\n%-30s: %.4f\n",
    data_name[0],initial_info[code_index].name,data_name[1], initial_info[code_index].code,
    data_name[2], initial_info[code_index].donator,data_name[3], initial_info[code_index].shipment_no,
    data_name[4], initial_info[code_index].quantity);
    menu();//go back to system main menu
}

void get_dist_data(int line_count)
{
    FILE * dist = fopen("dist.txt", "r");//open file in read mode
    char line[255];//declare variable needed
    fgets(line, 255, dist);//exclude the data column name
    dist_data data11[line_count];
    int row = 0;
    int count = 0;
    while(fgets(line, 255, dist) != NULL)
    {
        const char flag[2] = "|";//split line with preset punctuation
        char* token = strtok(line, flag);
        count = 0;
        while(token != NULL)
        {
            switch (count)//assign data into struct
            {
            case 0:
                strcpy(data11[row].name, token);
                break;
            case 1:
                strcpy(data11[row].code, token);
                break;
            case 2:
                strcpy(data11[row].hospital_name, token);
                break;
            case 3:
                data11[row].dist_quantity = atof(token);
                break;
            default:
                break;
            }
            count++;
            token = strtok(NULL, flag);
        }
        row++;
    }
    fclose(dist);//close the file opened
    bubble_sort(data11, line_count, 1);//sort the data in struct
}

void sorted_record_query()//start the data sorting using name
{
    char file_name[] = "dist.txt";
    int count = count_file_line(file_name);//count the number of line in file
    get_dist_data(count);//pass the data to next process
}

void start_data()
{
    int count = 0;
    while(count < 5)//enter only 5 items
    {
        printf("To restart the system you need to enter the items data.\n");
        printf("Please enter the details of item for item #%d: \n", count+1);
        fflush(stdin);
        printf("Donation name: ");//enter donator name
        fgets(initial_info[count].name, 64, stdin);
        initial_info[count].name[strlen(initial_info[count].name) - 1] = '\0';
        initial_info[count].name[0] = toupper(initial_info[count].name[0]);
        for(int i =0;i < strlen(initial_info[count].name);i++)
        {
            if(initial_info[count].name[i] == ' ')
                initial_info[count].name[i + 1] = toupper(initial_info[count].name[i + 1]);
        }//convert lowercase to uppercase
        fflush(stdin);//clear buffer
        printf("Supply code: ");
        scanf("%s", initial_info[count].code);//enter supply code
        for(int i = 0; i < strlen(initial_info[count].code);i++)//convert lowercase to uppercase
            initial_info[count].code[i] = toupper(initial_info[count].code[i]);
        fflush(stdin);//clear buffer
        printf("Donator: ");
        scanf("%s", initial_info[count].donator);//enter donator name
        initial_info[count].donator[0] = toupper(initial_info[count].donator[0]);
        for(int i =0;i < strlen(initial_info[count].donator);i++)
        {
            if(initial_info[count].donator[i] == ' ')
                initial_info[count].donator[i + 1] = toupper(initial_info[count].donator[i + 1]);
        }//convert lowercase to uppercase
        fflush(stdin);//clear buffer
        printf("Number of shipment: ");//enter number of shipment
        scanf("%d", &initial_info[count].shipment_no);
        fflush(stdin);//clear buffer
        while(1)
        {
            printf("Available quantity (in millions): ");
            scanf("%f", &initial_info[count].quantity);
            fflush(stdin);
            if(initial_info[count].quantity <= 0)
            {
                printf("Invalid input");
                continue;
            }//check input stock quantity
            else
                break;
        }
        printf("\nDetails of item:\n%s: %s\n%s: %s\n%s: %s\n%s: %d\n%s: %.2f million\n\n", 
        data_name[0], initial_info[count].name, data_name[1], initial_info[count].code,
        data_name[2], initial_info[count].donator, data_name[3], initial_info[count].shipment_no, 
        data_name[4], initial_info[count].quantity);//print item information
        count++;
    }
}

void restart()
{
    printf("\n");
    for(int i = 0; i < 5; i++)
        printf("--");
    printf("COVID-19 Donation Management system");//print system header
    for(int i = 0; i < 5; i++)
        printf("--");
    while(1)
    {
        printf("\n\nHi, do you want to restart the system? [Y for Yes, N for No]:");//ask user whether to restart the system
        fflush(stdin);
        char choice;
        scanf("%c", &choice);
        if(toupper(choice) == 'Y')//if the answer from user is yes
        {
            start_data();
            struct_write_file();
            dist_record_overwriting();//clear the system record
            menu();//return system menu
        }
        else if(toupper(choice) == 'N')//if user answe is not
        {
            printf("Restarting aborted.\n");
            read_txt_file();//utilize the data in file
            menu();//return system menu
        }
        else
        {
            printf("Invalid input. Please make your Choice again.");//reenter when getting invalid input
            fflush(stdin);
            continue;
        }
        break;
    }
}

void list_quantity_check()//check all the donation quantity in list
{
    for(int i = 0; i < 5; i++)
    {
        printf("\n%s: %s\n", data_name[0], initial_info[i].name);
        printf("%s: %s\n", data_name[1], initial_info[i].code);
        printf("%s: %s\n", data_name[2], initial_info[i].donator);
        printf("%s: %d\n", data_name[3], initial_info[i].shipment_no);
        printf("%s: %.2f million\n\n", data_name[4], initial_info[i].quantity);
    }
    menu();//return system menu
}

void menu()
{
    printf("\n");
    for(int i = 0; i < 5; i++)
        printf("--");
    printf("COVID-19 Donation Management system");//print the header of menu
    for(int i = 0; i < 5; i++)
        printf("--");
    printf("\n\
\t1. Donation receive\n\
\t2. Distribute donation\n\
\t3. Check donation quantity\n\
\t4. Check all donation quantity\n\
\t5. Check sorted distribution record\n\
\t6. Restarting system\n\
\t7. Exit\n\
Please make your choice: ");//show the option and ask user to choose
    int choice = 0;
    scanf("%d", &choice);
    fflush(stdin);
    switch (choice)//redirect user to function chosen
    {
        case 1:
            donation_input();
            break;
        case 2:
            distribution();
            break;
        case 3:
            quantity_check();
            break;
        case 4:
            list_quantity_check();
            break;
        case 5:
            sorted_record_query();
            break;
        case 6:
            restart();
            break;
        case 7:
            exit(EXIT_SUCCESS);
            break;
        default:
            printf("Invalid choice. Please reenter your choice.");//ask user to reenter when getting invalid input
            menu();
            break;
    }
}

int main()//start the system
{
    restart();//ask user whether to restart the system
    return 0;
}