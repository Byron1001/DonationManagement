#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#define _CRT_SECURE_NO_WARNINGS

struct supply
{
    char name[64];
    char code[3];
    char donator[64];
    int shipment_no;
    float quantity;
};//declare struct for supply details

struct supply initial_info[5] = {
    {"", "", "", 0, 0},
    {"", "", "", 0, 0},
    {"", "", "", 0, 0},
    {"", "", "", 0, 0},
    {"", "", "", 0, 0}
    };//declare the data for system using in normal operation

char data_name[5][36] = {
    "Name of supply",
    "Supply code",
    "Donator",
    "No. of Shipment",
    "Quantity Received (millions)"
};//declare the name of each supply details in donation file

char dist_data_name[5][36] = {
    "Name of supply",
    "Supply code",
    "Hospital Name",
    "Quantity distributed (thousands)"
};//declare the name of each supply details in distribution record file

typedef struct dist_data_struct
{
    char name[25];
    char code[3];
    char hospital_name[200];
    float dist_quantity;
} dist_data;//declare the struct for distribution details

dist_data accu_array[5] = {
    {"Contactless Thermometer", "CT", "", 0},
    {"Hand Sanitizers", "HS", "", 0},
    {"Face Mask", "FM", "", 0},
    {"Surgical Mask", "SM", "", 0},
    {"Oxygen Mask", "OM", "", 0}
    };//declare the data for distribution operation

void menu(void);//declare prototype of the function
void struct_write_file(void);
void accumulative(dist_data data[], int line_count);

double get_paste_line_quantity(char line[255], int index)//get the quantity number from the received line data
{
    char char_quantity[50];//declare variable needed
    int start = 76;
    int cq_index = 0;
    while (isdigit(line[start]) || line[start] == '.')//get the number and punctuation '.' when it is float
    {
        char_quantity[cq_index] = line[start];//save to variable
        start++;
        cq_index++;
    }
    double quantity = atof(char_quantity);//convert string to float
    initial_info[index].quantity = quantity;//save to struct
    return quantity;
}

int get_paste_line_shipment(char line[255], int index)//get the number of shipment from the received line data
{
    char char_shipment[4];//declare variable needed
    int start = 58;
    int cs_index = 0;
    while (isdigit(line[start]))//get the digit only
    {
        char_shipment[cs_index] = line[start];//save to variable
        start++;
        cs_index++;
    }
    int shipment = atoi(char_shipment);//convert string to integer
    initial_info[index].shipment_no = shipment;//save to struct
    return shipment;
}

void get_paste_line_code(char line[255], int index)//get the supply code from the received line data
{
    char char_code[3] = {0};//declare variable needed
    int start = 26;
    int cc_index = 0;
    while (isalpha(line[start]))
    {
        char_code[cc_index] = line[start];//save to variable
        start++;
        cc_index++;
    }
    char_code[cc_index] = '\0';
    strcpy(initial_info[index].code, char_code);//save data to struct
}

void get_paste_line_donator(char line[255], int index)//get the donator name from the received line data
{
    char char_donator[17];//declare variable needed
    int start = 42;
    int cd_index = 0;
    int flag = 0;
    while (flag < 2 && (isalpha(line[start]) == 1 || isspace(line[start]) == 1))
    {
        char_donator[cd_index] = line[start];//save to variable
        start++;
        cd_index++;
        if(line[start] == ' ')//sentinnel for space allowed in data
            flag++;
    }
    for(int i = cd_index; i <= strlen(char_donator); i++)
    {
        strcpy(&char_donator[i], "");
    }
    strcpy(initial_info[index].donator, char_donator);//save data to struct
}

void get_paste_line_name(char line[255], int index)//get the name of supply from the received line data
{
    char name[25];//declare variable needed
    int start = 0;
    int cn_index = 0;
    int flag = 0;
    while (flag < 2 && (isalpha(line[start]) == 1 || isspace(line[start]) == 1))
    {
        name[cn_index] = line[start];//save to variable
        start++;
        cn_index++;
        if(line[start] == ' ')
            flag++;
    }
    for(int i = cn_index; i <= strlen(name); i++)
    {
        strcpy(&name[i], "");
    }
    strcpy(initial_info[index].name, name);//save the data to struct
}

void print_supply_code_list()//print the supply code in system to be chosen
{
    printf("\n");
    for(int i = 0; i < 30; i++)
    {
        printf("*");
    }
    printf("\n\tChoose %s:\n",data_name[1]);
    for(int i = 0; i < 5; i++)
    {
        printf("%-23s : %s \n", initial_info[i].name, initial_info[i].code);
    }//print the supply code and name of supply in list
}

void donation_quantity_add(int code_index, int shipment, float quantity)//add the quantity received
{
    initial_info[code_index].shipment_no = shipment;
    initial_info[code_index].quantity += quantity;//add the quantity
    printf("Donation input successful.\n");////inform user that the data is successful updated
    struct_write_file();//overwrite the file with new struct data
    menu();//return to system menu
}

void quantity_modify_confirm(int shipment, float quantity, int code_index)//send confirmation for donation adding
{
    printf("\nDonation received checking: \n");
    printf("%-30s : %s\n%-30s : %s\n%-30s : %d\n%-30s : %.3f million\n", data_name[0], initial_info[code_index].name, 
    data_name[1], initial_info[code_index].code, data_name[3], shipment, data_name[4], quantity);//print the data which is going to be used in modification
    while (1)
    {
        printf("\nPlease confirm your input ('Y' for Yes, 'N' for no): ");//ask user for confirmation
        char choice;
        scanf("%c", &choice);
        if(toupper(choice) == 'Y')//if user answer is yes
            donation_quantity_add(code_index, shipment, quantity);//start add the quantity received
        else if(toupper(choice) == 'N')//if user answer is no
        {
            printf("Donation Received cancelled.");
            menu();//return systme menu
            return ;//stop function
        }
        else
        {
            printf("Invalid input. Please make your Choice again.");//reenter choice if the input is invalid
            fflush(stdin);//clear the buffer
            continue;
        }
        break;
    }
}

int supply_code_input()//getting supply code and check the input
{
    print_supply_code_list();//print the supply code list
    printf("\nPlease enter supply code: ");
    char get_code[3];
    fflush(stdin);
    scanf("%s", get_code);
    fflush(stdin);
    for (int i = 0; i < strlen(get_code); i++)
    {
        char word = toupper(get_code[i]);//conver the input to uppercase
        get_code[i] = word;
    }
    get_code[2] = '\0';
    int flag = 0;
    int code_index;
    for (int i = 0; i < 5; i++)
    {
        if (strcmp(get_code, initial_info[i].code) == 0)//compare the input and the existing supply code
        {
            code_index = i;
            flag = 1;
        }
        else if(i == 4 && flag == 0)
        {
            printf("\nInvalid supply code.\nPlease reenter supply code.\n");//reenter when getting invalid supply code
            fflush(stdin);//clear buffer
            supply_code_input();//call the input session again
        }
        else if(flag == 1)
            break;
    }
    fflush(stdin);
    return code_index;//return the index of supply code in struct
}

void dist_record_overwriting()//overwrit the distribution file
{
    FILE * distribution = fopen("dist.txt", "w");//open file in write mode
    fprintf(distribution, "%s|%s|%s|%s\n",
    dist_data_name[0], dist_data_name[1], dist_data_name[2], dist_data_name[3]);//print the column name of data into file
    printf("Distribution record overwritten.\n");//inform user that the file is successful updated
    fclose(distribution);//close the file opened
}

void dist_record_add(int code_index, float quantity, char hospital[200])
{
    FILE * dist = fopen("dist.txt", "a");//open file in append mode
    fprintf(dist, "%s|%s|%s|%.3f\n", 
    initial_info[code_index].name, initial_info[code_index].code, hospital, quantity*1000);//append the data into file
    printf("Distribution recorded.\n");//inform user that the data is successful updated
    fclose(dist);//close the file opened
}

void printf_sort_struct(dist_data data[], int line_count)//print the data in sorted struct
{
    printf("\n");
    for(int line = 0; line < line_count; line++)//print data in struct
    {
        printf("%-35s:%s\n", dist_data_name[0], data[line].name);
        printf("%-35s:%s\n", dist_data_name[1], data[line].code);
        printf("%-35s:%s\n", dist_data_name[2], data[line].hospital_name);
        printf("%-35s:%.3f\n\n", dist_data_name[3], data[line].dist_quantity);
    }
    printf("All record finished printed.\n");//inform user that the data print is finished
}

void printf_acc_struct(dist_data data[5])//print the accumulative quantity of supply in list
{
    printf("\nAccumulative quantity:\n");
    for(int line = 0; line < 5; line++)
    {
        printf("%-25s (%s):%10.2f thousand\n",
        data[line].name, data[line].code, data[line].dist_quantity);
    }
    for(int i = 0; i < 5; i++)
        data[i].dist_quantity = 0;//reset the accumulative quantity, preventing wrong output user recalls function
    menu();//return system menu
}

void bubble_sort(dist_data data[], int line_count, int acc)//use bubble sort to sort the struct data
{
    dist_data array;
    for(int times = 0; times < line_count; times++)
    {
        for(int cmp = 0; cmp < line_count; cmp++)
        {
            if(data[cmp].dist_quantity < data[cmp + 1].dist_quantity & cmp < line_count - 1)
            {
                array = data[cmp];
                data[cmp] = data[cmp + 1];
                data[cmp + 1] = array;
            }
        }
    }
    if(acc == 1)//if it is need to count the accumulative quantity
    {
        printf_sort_struct(data, line_count);//print sorted struct
        accumulative(data, line_count);//proceed to accumulative calculation
    }
    else
    {
        printf_acc_struct(data);//print accumulative quantity in list
    }
}

void accumulative(dist_data data[], int line_count)//count the accumulative quantity of distribution
{
    for(int times = 0; times < 5; times++)
    {
        for(int cmp = 0; cmp < line_count; cmp++)
        {
            if(strcmp(accu_array[times].code, data[cmp].code) == 0)
            {
                accu_array[times].dist_quantity += data[cmp].dist_quantity;
            }
        }
    }
    bubble_sort(accu_array,5, 0);//call back bubble sort function
}

int count_file_line(char file_name[])//count the line in file
{
    FILE * dist = fopen(file_name, "r");
    char line[255];
    fgets(line, 255, dist);
    int line_count = 0;
    while (fgets(line, 255, dist) != NULL)
    {
        line_count++;
    }
    fclose(dist);
    return line_count;//return the line number of file
}

void quantity_dist_confirm(float quantity, int code_index, char hospital[])//send confirmation to user for the distribution data
{
    printf("\nDistribution checking: \n");
    printf("%-35s : %s\n%-35s : %s\n%-35s : %s\n%-35s : %.3f thousand\n", dist_data_name[0], initial_info[code_index].name, 
    dist_data_name[1], initial_info[code_index].code, dist_data_name[2], hospital, dist_data_name[3], quantity*1000);//print the distribution data from user
    while (1)
    {
        printf("\nPlease confirm your distribution ('Y' for Yes, 'N' for no): ");//ask user for confirmation
        char choice;
        scanf("%c", &choice);
        if(toupper(choice) == 'Y')//if the user answer yes
        {
            initial_info[code_index].quantity -= quantity;
            printf("Quantity distributed.\n");//inform user that the data is successful updated
            dist_record_add(code_index, quantity, hospital);
            struct_write_file();//write the file with new struct data
            menu();//return system menu
        }
        else if(toupper(choice) == 'N')//if user answer is no
        {
            printf("Donation Received cancelled.");
            menu();//return system menu
        }
        else
        {
            printf("Invalid input. Please make your Choice again.");//reenter choice when getting invalid input
            fflush(stdin);//clear buffer
            continue;
        }
        break;
    }
}
