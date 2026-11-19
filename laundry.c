#include <stdio.h>      /* for printf, scanf, fopen, fprintf, fscanf, fclose */
#include <string.h>     /* for strlen (counts the characters in a phone number) */

#define MAX_ORDERS 100          /* the program can store up to 100 orders */
#define FILE_NAME  "laundry.txt" /* the file where orders are saved */

/* One order */
struct Order {
    int   id;                   /* order number */
    char  name[50];             /* customer name */
    char  phone[16];            /* phone number */
    int   service;              /* 1 = Wash, 2 = Dry Clean, 3 = Iron */
    int   garments;             /* number of clothes */
    float amount;               /* price in Rs. */
    int   status;               /* 0 = Received, 1 = Washing, 2 = Ready, 3 = Delivered */
};

struct Order orders[MAX_ORDERS];   /* list of all orders */
int count = 0;                     /* how many orders we have now */
int nextId = 1;                    /* number for the next new order */

/* Gives the price for one garment */
int rateFor(int service) {
    /* Wash costs 100, Dry Clean costs 300, Iron costs 50 */
    return service == 1 ? 100 : (service == 2 ? 300 : 50);
}

/* Asks the user for a number until it is in the allowed range */
int askInt(const char *question, int min, int max) {
    int value = -1;             /* the number the user types */
    int c;                      /* one character at a time */
    do {                        /* repeat until the answer is correct */
        printf("%s", question); /* show the question */
        if (scanf("%d", &value) != 1) {   /* read a number; if the user typed letters... */
            value = -1;         /* ...set it to a wrong value */
        }
        while ((c = getchar()) != '\n' && c != EOF) {  /* remove the rest of the line */
        }
        if (value < min || value > max) {  /* the number is too small or too big */
            printf("Enter a number from %d to %d.\n", min, max);  /* show the allowed range */
        }
    } while (value < min || value > max);  /* ask again if it is wrong */
    return value;               /* send back the correct number */
}

/* Finds an order by its number. Gives back its position, or -1 if not found */
int findOrder(int id) {
    int i;                      /* counter for the loop */
    for (i = 0; i < count; i++) {   /* check each order one by one */
        if (orders[i].id == id) {   /* this is the order we want */
            return i;           /* send back its position */
        }
    }
    return -1;                  /* not found */
}

/* Prints one order on one line */
void printOrder(struct Order *o) {
    /* the names for each service number */
    char *services[] = {"", "Wash", "Dry Clean", "Iron"};
    /* the names for each status number */
    char *statuses[] = {"Received", "Washing", "Ready", "Delivered"};

    /* print all the fields, each in a fixed-width box so the columns line up */
    /* %-4d = number in 4 characters, %-15s = text in 15 characters, and so on */
    printf("%-4d %-15s %-13s %-10s %-9d Rs.%-11.2f %s\n", o->id, o->name, o->phone,
           services[o->service], o->garments, o->amount, statuses[o->status]);
}

/* Shows the title row and then all orders */
void showOrders(void) {
    int i;                      /* counter for the loop */
    if (count == 0) {           /* there are no orders */
        printf("No orders yet.\n");   /* say so */
        return;                 /* stop here */
    }
    /* print the column titles, each in a fixed-width box so they line up */
    /* %-4s = 4 characters, %-15s = 15 characters, and so on; - means left align */
    printf("%-4s %-15s %-13s %-10s %-9s %-14s %s\n",
           "ID", "Name", "Phone", "Service", "Garments", "Amount", "Status");
    for (i = 0; i < count; i++) {   /* go through every order */
        printOrder(&orders[i]);     /* print order number i */
    }
}

/* Asks for the details of a new order and saves it */
void addOrder(void) {
    struct Order *o;            /* will point to the new order */

    if (count == MAX_ORDERS) {  /* the list is full */
        printf("Storage is full.\n");   /* say so */
        return;                 /* stop here */
    }

    o = &orders[count];         /* point to the next empty spot */
    o->id = nextId;             /* give it the next number */

    printf("Customer name: ");  /* ask for the name */
    scanf(" %49[^\n]", o->name);  /* read the whole line, spaces allowed */

    do {                        /* repeat until the phone number is long enough */
        printf("Phone (10-15 digits): ");   /* ask for the phone number */
        scanf("%15s", o->phone);            /* read the phone number */
        if (strlen(o->phone) < 10) {        /* less than 10 digits */
            printf("Phone is too short.\n");  /* say so */
        }
    } while (strlen(o->phone) < 10);        /* ask again if it is too short */

    /* ask for the service: 1, 2 or 3 */
    o->service = askInt("Service (1 Wash, 2 Dry Clean, 3 Iron): ", 1, 3);
    /* ask for the number of clothes: 1 to 200 */
    o->garments = askInt("Number of garments (1-200): ", 1, 200);
    o->amount = o->garments * rateFor(o->service);  /* price = clothes x price per cloth */
    o->status = 0;              /* a new order starts as Received */

    count++;                    /* we have one more order */
    nextId++;                   /* the next order gets the next number */
    printf("Order #%d added. Amount due: Rs. %.2f\n", o->id, o->amount);  /* show the result */
}

/* Finds one order by its number and shows it */
void searchOrder(void) {
    /* ask for the number and find where that order is in the list */
    int i = findOrder(askInt("Order ID: ", 1, 100000));
    if (i == -1) {              /* not found */
        printf("Order not found.\n");   /* say so */
    } else {                    /* found */
        printOrder(&orders[i]); /* show it */
    }
}

/* Changes the status of one order */
void updateStatus(void) {
    /* ask for the number and find where that order is in the list */
    int i = findOrder(askInt("Order ID: ", 1, 100000));
    if (i == -1) {              /* not found */
        printf("Order not found.\n");   /* say so */
        return;                 /* stop here */
    }
    /* ask for the new status (0 to 3) and save it in the order */
    orders[i].status = askInt("New status (0 Received, 1 Washing, "
                              "2 Ready, 3 Delivered): ", 0, 3);
    printf("Status updated.\n");        /* say it worked */
}

/* Removes one order from the list */
void deleteOrder(void) {
    /* ask for the number and find where that order is in the list */
    int i = findOrder(askInt("Order ID to delete: ", 1, 100000));
    int j;                      /* counter for the loop */

    if (i == -1) {              /* not found */
        printf("Order not found.\n");   /* say so */
        return;                 /* stop here */
    }
    for (j = i; j < count - 1; j++) {  /* move every order after it up one spot */
        orders[j] = orders[j + 1];     /* copy the next order into this spot */
    }
    count--;                    /* we now have one order fewer */
    printf("Order deleted.\n"); /* say it worked */
}

/* Shows the total orders, the total money, and how many are in each status */
void report(void) {
    int i;                      /* counter for the loop */
    float total = 0;            /* adds up all the money */
    int perStatus[4] = {0, 0, 0, 0};  /* how many orders are in each status */

    for (i = 0; i < count; i++) {     /* check every order */
        total += orders[i].amount;    /* add this order's price to the total */
        perStatus[orders[i].status]++;  /* count this order under its status */
    }
    printf("Orders: %d   Revenue: Rs. %.2f\n", count, total);  /* first line */
    /* second line: how many in each status */
    printf("Received %d | Washing %d | Ready %d | Delivered %d\n",
           perStatus[0], perStatus[1], perStatus[2], perStatus[3]);
}

/* Writes all orders to the file, one order per line */
void saveOrders(void) {
    FILE *fp = fopen(FILE_NAME, "w");  /* open the file to write (old data is replaced) */
    int i;                      /* counter for the loop */

    if (fp == NULL) {           /* the file could not be opened */
        printf("Cannot open file to save.\n");  /* say so */
        return;                 /* stop here */
    }
    for (i = 0; i < count; i++) {   /* go through every order */
        /* write one order to the file, with | between the fields */
        fprintf(fp, "%d|%s|%s|%d|%d|%.2f|%d\n", orders[i].id, orders[i].name,
                orders[i].phone, orders[i].service, orders[i].garments,
                orders[i].amount, orders[i].status);
    }
    fclose(fp);                 /* close the file so it is saved */
    printf("Records saved.\n"); /* say it worked */
}

/* Reads the orders from the file when the program starts */
void loadOrders(void) {
    FILE *fp = fopen(FILE_NAME, "r");  /* open the file to read */
    struct Order *o;            /* will point to the order we read */

    if (fp == NULL) {           /* no file yet (first time running) */
        return;                 /* nothing to load, so stop */
    }
    while (count < MAX_ORDERS) {    /* keep reading while there is room */
        o = &orders[count];         /* point to the next empty spot */
        /* read one line in the same | format; stop if it is wrong */
        if (fscanf(fp, "%d|%49[^|]|%15[^|]|%d|%d|%f|%d\n", &o->id, o->name,
                   o->phone, &o->service, &o->garments, &o->amount,
                   &o->status) != 7) {
            break;              /* no more good lines, so stop */
        }
        if (o->id >= nextId) {  /* make sure new numbers are bigger than loaded ones */
            nextId = o->id + 1; /* the next new order gets a bigger number */
        }
        count++;                /* we have one more order */
    }
    fclose(fp);                 /* close the file */
    printf("Loaded %d order(s).\n", count);  /* say how many were loaded */
}

/* The program starts here */
int main(void) {
    int choice;                 /* the menu option the user picks */

    loadOrders();               /* load saved orders first */

    do {                        /* show the menu again and again until the user exits */
        /* show the menu */
        printf("\n1. Add order\n2. Show all orders\n3. Search by ID\n");
        printf("4. Update status\n5. Delete order\n6. Summary report\n");
        printf("0. Save and exit\n");

        choice = askInt("Choice: ", 0, 6);  /* ask for an option from 0 to 6 */

        switch (choice) {       /* run the right part for the choice */
            case 1: addOrder();     break;  /* add an order */
            case 2: showOrders();   break;  /* show all orders */
            case 3: searchOrder();  break;  /* search by number */
            case 4: updateStatus(); break;  /* change the status */
            case 5: deleteOrder();  break;  /* delete an order */
            case 6: report();       break;  /* show the summary */
            case 0: saveOrders();   break;  /* save before quitting */
        }
    } while (choice != 0);      /* stop when the user picks 0 */

    return 0;                   /* the program ended normally */
}
