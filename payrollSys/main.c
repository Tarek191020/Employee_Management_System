#include <stdio.h>
#include <stdlib.h>

#define MAX_EMPLOYEES 100

typedef struct
{
    char name[50];
    int id ;
    double salary_per_hour;
    double hours_rate;
    double totalSalary;
    double grossSalary;
    double netsalary;
}Employee_t;

/*********************************Global Variables**********************************/
Employee_t Employees[MAX_EMPLOYEES];
int employeeCount = 0;
FILE *DataBase;

/*********************************Function Prototypes**********************************/
double Calc_Tax(float salary);
double Calculate_GrossSalary(float totalSalary);
double Calculate_NetSalary(float Total_salary);
void Add_Emp();
void Delete_Emp(int id);
void Display_Emp(int id);
void Display_All_Employees(void);
void Compute_Payroll(void);
void Load_Employees_From_File(void);
void Application(void);

/*********************************Main Fun**********************************/
int main()
{
    Application();
    return 0;
}

/*********************************Functions Implementation**********************************/
// Function to calculate tax based on salary
double Calc_Tax(float salary)
{
    float tax;
    if(salary >= 0.0 && salary <15000.00)
    {
        tax = salary * 0.15;
    }
    else if(salary >= 15000.0 && salary <30000.0)
    {
        tax = 2250.0 + ((salary-15000.0)*0.16);
    }
    else if(salary >= 30000.0 && salary <50000.0)
    {
        tax = 4650.0 + ((salary-30000.0)*0.18);
    }
    else if(salary >= 50000.0 && salary <80000.0)
    {
        tax = 8250.0 + ((salary-50000.0)*0.2);
    }
    else if(salary >= 80000.0 && salary <=150000.0)
    {
        tax = 14250.0 + ((salary-80000.0)*0.25);
    }
    else // Invalid salary
    {
        tax = -1;
    }
    return tax;
}

// Function to calculate gross salary
double Calculate_GrossSalary(float totalSalary)
{
    float incentives = totalSalary * 0.52;
    return totalSalary + incentives;
}

// Function to calculate net salary (gross salary - tax)
double Calculate_NetSalary(float Total_salary)
{
    float net;
    if(Calc_Tax(Total_salary)==-1)//Invalid salary
    {
        net = -1;
    }
    else
    {
        net = Total_salary + (Total_salary * 0.52) - Calc_Tax(Total_salary);
    }
    return net ;
}

// Function to add a new employee
void Add_Emp(void)
{
    if (employeeCount >= MAX_EMPLOYEES)
    {
        printf("Error: Cannot add more employees.\n");
    }
    else
    {
        Employee_t *emp = &Employees[employeeCount];
        emp->id = employeeCount + 1;
        getchar();
        printf("Employee ID: %d\n", emp->id);
        printf("Enter Employee Name: ");
        scanf(" %[^\n]", emp->name);  // Read name until newline
        printf("Enter Hours Worked: ");
        scanf("%lf", &emp->hours_rate);
        printf("Enter Hourly Rate: ");
        scanf("%lf", &emp->salary_per_hour);

        // Calculate salaries
        emp->totalSalary = emp->hours_rate * emp->salary_per_hour;
        emp->grossSalary = Calculate_GrossSalary(emp->totalSalary);
        emp->netsalary = Calculate_NetSalary(emp->totalSalary);

        if(emp->netsalary == -1)
        {// Handle invalid salary
            printf("salary problem\n");
            printf("Enter Hours Worked again: ");
            scanf("%lf", &emp->hours_rate);
            printf("Enter Hourly Rate again: ");
            scanf("%lf", &emp->salary_per_hour);

            emp->totalSalary = emp->hours_rate * emp->salary_per_hour;
            emp->grossSalary = Calculate_GrossSalary(emp->totalSalary);
            emp->netsalary = Calculate_NetSalary(emp->totalSalary);
        }

        printf("Employee added successfully.\n");
        employeeCount++;

        // Save employee data to file
        DataBase = fopen("employees.txt", "a");  // Open file in append mode
        if (DataBase != NULL)
        {
            fprintf(DataBase, "%d\t%s\t%lf\t%lf\t%lf\t%lf\t%lf\n",
                    emp->id,
                    emp->name,
                    emp->hours_rate,
                    emp->salary_per_hour,
                    emp->totalSalary,
                    emp->grossSalary,
                    emp->netsalary);
            fclose(DataBase);
        }
        else
        {
            printf("Error: Could not save employee to file.\n");
        }
    }
}

// Function to delete an employee by Id
void Delete_Emp(int id)
{
    int found = 0;
    DataBase = fopen("employees.txt", "r");
    if (DataBase == NULL)
    {
        printf("Error: Could not open file for reading.\n");
        return;
    }
    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL)
    {
        printf("Error: Could not open temporary file for writing.\n");
        fclose(DataBase);
        return;
    }
    for (int i = 0; i < employeeCount; i++)
    {
        if (Employees[i].id == id)
        {// Skip the employee to delete
            found = 1;
            continue;
        }
        // Write other employees to the temporary file
        fprintf(tempFile, "%d\t%s\t%lf\t%lf\t%lf\t%lf\t%lf\n",
                Employees[i].id,
                Employees[i].name,
                Employees[i].hours_rate,
                Employees[i].salary_per_hour,
                Employees[i].totalSalary,
                Employees[i].grossSalary,
                Employees[i].netsalary);
    }
    fclose(DataBase);
    fclose(tempFile);

    if (found)
    {
        // Replace the original file with the temporary file
        remove("employees.txt");
        rename("temp.txt", "employees.txt");
        // Remove the employee from the global array
        for (int i = 0; i < employeeCount; i++)
        {
            if (Employees[i].id == id)
            {
                for (int j = i; j < employeeCount - 1; j++)
                {
                    Employees[j] = Employees[j + 1];
                }
                employeeCount--;
                break;
            }
        }
        printf("Employee with Id %d deleted successfully.\n", id);
    }
    else
    {
        remove("temp.txt");  // Clean up temporary file
        printf("Error: Employee with Id %d not found.\n", id);
    }
}

// Function to display a single employee by Id
void Display_Emp(int id)
{
    int found = 0;
    for (int i = 0; i<employeeCount; i++)
    {
        if(Employees[i].id == id)
        {
            found = 1;
            printf("-------------------------------\n");
            printf("Employee Id: %d",id);
            printf("\nName: %s",Employees[i].name);
            printf("\nhours worked: %lf",Employees[i].hours_rate);
            printf("\nSalary per hour: %lf",Employees[i].salary_per_hour);
            printf("\nTotal Salary: %lf",Employees[i].totalSalary);
            printf("\nGross Salary: %lf",Employees[i].grossSalary);
            printf("\nNet salary: %lf\n",Employees[i].netsalary);
            printf("-------------------------------\n");
        }
    }
    if (!found)
    {
        printf("Error: Employee with Id %d not found.\n", id);
    }
}

// Function to display all employees
void Display_All_Employees(void)
{
    if (employeeCount == 0)
    {
        printf("No employees to display.\n");
        return;
    }
    printf("\nAll Employees:\n");
    printf("--------------------------------------------------------------------------------------------\n");
    printf("Id\tName\tWork Hours\tprice per hour\tTotal Salary\tGross Salary\tNet Salary\n");
    printf("--------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < employeeCount; i++) {
        printf("%d\t%s\t%lf\t%lf\t",Employees[i].id,Employees[i].name,Employees[i].hours_rate,Employees[i].salary_per_hour);
        printf("%lf\t%lf\t%lf\n", Employees[i].totalSalary,Employees[i].grossSalary,Employees[i].netsalary);
    }
}

// Function to compute total company payroll
void Compute_Payroll(void)
{
    float totalPayroll = 0.0;
    for (int i = 0; i < employeeCount; i++)
    {
        totalPayroll += Employees[i].netsalary;
    }
    printf("\nTotal Company Payroll: %.2f\n\n", totalPayroll);
}

// Function to load employees from file
void Load_Employees_From_File(void)
{
    DataBase = fopen("employees.txt", "r");  // Open file in read mode
    if (DataBase != NULL)
    {
        char line[256];  // Buffer for each line
        while (fgets(line, sizeof(line), DataBase))
        {
            Employee_t *emp = &Employees[employeeCount];

            // Parse the line into employee structure
            sscanf(line, "%d\t%s\t%lf\t%lf\t%lf\t%lf\t%lf",
                   &emp->id,
                   emp->name,
                   &emp->hours_rate,
                   &emp->salary_per_hour,
                   &emp->totalSalary,
                   &emp->grossSalary,
                   &emp->netsalary);

            employeeCount++;  // Increment employee count
        }
        fclose(DataBase);
    }
    else
    {
        printf("No existing employee data found. Starting fresh.\n");
    }
}

// Application Function
void Application()
{
    int choice, id;
    Load_Employees_From_File();     // Load existing employees
    while (1)
    {
        printf("\nWelecome to Employee Management System\n");
        printf("1. Add Employee\n");
        printf("2. Delete Employee\n");
        printf("3. Display Employee\n");
        printf("4. Display All Employees\n");
        printf("5. Compute Payroll\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
            case 1:
                Add_Emp();
                break;
            case 2:
                printf("Enter Employee Id to delete: ");
                scanf("%d", &id);
                Delete_Emp(id);
                break;
            case 3:
                printf("Enter Employee Id to display: ");
                scanf("%d", &id);
                Display_Emp(id);
                break;
            case 4:
                Display_All_Employees();
                break;
            case 5:
                Compute_Payroll();
                break;
            case 6:
                printf("Exiting program. Goodbye\n");
                return;
            default:
                printf("Invalid choice. Try again.\n");
        }
    }
}
