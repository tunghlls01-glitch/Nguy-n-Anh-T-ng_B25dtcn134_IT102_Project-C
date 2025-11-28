#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

struct Account {
    char accountId[20];
    char fullName[50];
    char phone[15];
    double balance;
    int status;
};

struct Transaction {
    char transId[20];
    char senderId[20];
    char receiverId[20];
    double amount;
    char type[10];
    char date[20];
};

void displayMenu (){ // ham hien thi menu
    printf("\n||=========Mini Banking System========||\n");
    printf("||       1. Them tai khoan moi        ||\n");
    printf("||       2. Cap nhat thong tin        ||\n");
    printf("||       3. Quan ly trang thai        ||\n");
    printf("||       4. Tra cuu(tim kiem)         ||\n");
    printf("||       5. Danh sach                 ||\n");
    printf("||       6. Sap xep danh sach         ||\n");
    printf("||       7. Giao dich chuyen khoan    ||\n");
    printf("||       8. Lich su giao dich         ||\n");
    printf("||       9. Thoat                     ||\n");
    printf("||====================================||\n");
}

void clear_stdin_line(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int isLetterString(const char *str){
    for(int i = 0; str[i] != '\0'; i++){
        if(!( (str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
               str[i] == ' ' )){
            return 0; // co ky tu khong hop le
        }
    }
    return 1; // hop le
}

void f01_addAccount(struct Account accounts[], int *size) {
    int addCount;
    char input[50];

    // ===== NHAP SO LUONG TAI KHOAN =====
    while (1) {
        printf("Nhap so luong tai khoan: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Loi khi nhap! Hay thu lai.\n");
            continue;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            printf("Khong duoc de trong! Hay nhap lai.\n");
            continue;
        }

        int hasNonSpace = 0;
        for (int i = 0; input[i]; i++) {
            if (!isspace((unsigned char)input[i])) {
                hasNonSpace = 1;
                break;
            }
        }
        if (!hasNonSpace) {
            printf("Khong duoc chi nhap khoang trang! Hay nhap lai.\n");
            continue;
        }

        int isNumber = 1;
        for (int i = 0; input[i]; i++) {
            if (!isdigit((unsigned char)input[i])) {
                isNumber = 0;
                break;
            }
        }
        if (!isNumber) {
            printf("Chi duoc nhap so! Hay nhap lai.\n");
            continue;
        }

        addCount = atoi(input);
        if (addCount <= 0 || addCount > 100) {
            printf("So luong khong hop le! (1..100)\n");
            continue;
        }
        if (*size + addCount > 100) {
            printf("Khong du suc chua de them %d tai khoan. Con lai: %d\n", addCount, 100 - *size);
            continue;
        }

        break; // hop le
    }

    // ===== THEM TAI KHOAN =====
    for (int k = 0; k < addCount; k++) {
        int idx = *size + k;
        printf("----------Tai khoan nguoi dung %d ----------\n", idx + 1);

        // ===== NHAP ID =====
        while (1) {
            char tmpId[sizeof(accounts[idx].accountId)];
            printf("Nhap ID (ma tai khoan): ");
            if (fgets(tmpId, sizeof(tmpId), stdin) == NULL) tmpId[0] = '\0';
            tmpId[strcspn(tmpId, "\n")] = '\0';

            if (strlen(tmpId) == 0) {
                printf("ID khong duoc de trong! Vui long nhap lai.\n");
                continue;
            }

            int hasNonSpace = 0;
            for (int i = 0; tmpId[i]; i++) {
                if (!isspace((unsigned char)tmpId[i])) {
                    hasNonSpace = 1;
                    break;
                }
            }
            if (!hasNonSpace) {
                printf("ID khong duoc chi co khoang trang! Hay nhap lai.\n");
                continue;
            }

            int hasInvalidChar = 0;
            for (size_t p = 0; p < strlen(tmpId); p++) {
                if (!isalnum((unsigned char)tmpId[p])) {
                    hasInvalidChar = 1;
                    break;
                }
            }
            if (hasInvalidChar) {
                printf("ID chi duoc chua chu va so! Khong duoc chua ky tu dac biet.\n");
                continue;
            }

            int isDuplicate = 0;
            int totalUsed = *size + k;
            for (int j = 0; j < totalUsed; j++) {
                if (strcmp(tmpId, accounts[j].accountId) == 0) {
                    isDuplicate = 1;
                    break;
                }
            }
            if (isDuplicate) {
                printf(">> Loi: ID nay da ton tai! Hay nhap ID khac.\n");
                continue;
            }

            strncpy(accounts[idx].accountId, tmpId, sizeof(accounts[idx].accountId) - 1);
            accounts[idx].accountId[sizeof(accounts[idx].accountId) - 1] = '\0';
            break;
        }

        // ===== NHAP TEN =====
        while (1) {
            printf("Nhap ten (FullName): ");
            if (fgets(accounts[idx].fullName, sizeof(accounts[idx].fullName), stdin) == NULL)
                accounts[idx].fullName[0] = '\0';
            accounts[idx].fullName[strcspn(accounts[idx].fullName, "\n")] = '\0';

            if (strlen(accounts[idx].fullName) == 0) {
                printf("Ten khong duoc de trong!\n");
                continue;
            }

            int hasLetter = 0;
            for (int i = 0; accounts[idx].fullName[i]; i++) {
                if (isalpha((unsigned char)accounts[idx].fullName[i])) {
                    hasLetter = 1;
                    break;
                }
            }
            if (!hasLetter) {
                printf("Ten khong duoc chi co khoang trang! Hay nhap lai.\n");
                continue;
            }

            if (!isLetterString(accounts[idx].fullName)) {
                printf("Ten chi duoc chua chu cai va khoang trang!\n");
                continue;
            }

            break;
        }

        // ===== NHAP SO DIEN THOAI =====
        while (1) {
            printf("Nhap so dien thoai (phone): ");
            if (fgets(accounts[idx].phone, sizeof(accounts[idx].phone), stdin) == NULL)
                accounts[idx].phone[0] = '\0';
            accounts[idx].phone[strcspn(accounts[idx].phone, "\n")] = '\0';

            if (strlen(accounts[idx].phone) == 0) {
                printf("So dien thoai khong duoc de trong!\n");
                continue;
            }

            int hasNonSpacePhone = 0;
            for (int i = 0; accounts[idx].phone[i]; i++) {
                if (!isspace((unsigned char)accounts[idx].phone[i])) {
                    hasNonSpacePhone = 1;
                    break;
                }
            }
            if (!hasNonSpacePhone) {
                printf("So dien thoai khong duoc chi co khoang trang!\n");
                continue;
            }

            int hasNonDigit = 0;
            for (size_t p = 0; p < strlen(accounts[idx].phone); p++) {
                if (!isdigit((unsigned char)accounts[idx].phone[p])) {
                    hasNonDigit = 1;
                    break;
                }
            }
            if (hasNonDigit) {
                printf("So dien thoai chi duoc chua chu so!\n");
                continue;
            }

            int duplicated = 0;
            int totalUsed = *size + k;
            for (int j = 0; j < totalUsed; j++) {
                if (strcmp(accounts[j].phone, accounts[idx].phone) == 0) {
                    duplicated = 1;
                    break;
                }
            }
            if (duplicated) {
                printf("So dien thoai da ton tai, vui long nhap so khac!\n");
                continue;
            }

            break;
        }

        accounts[idx].balance = 100000.0;
        accounts[idx].status = 1;
    }

    *size += addCount;
    printf("Them tai khoan thanh cong! Tong so hien tai: %d\n", *size);
}

void f02_updateAccount(struct Account accounts[], int size) {
    if (size == 0) {
        printf("Chua co tai khoan nao trong he thong!\n");
        return;
    }

    char id[sizeof(accounts[0].accountId)];
    int found = -1;

    // ===== Nhap ID =====
    while (1) {
        printf("Nhap ID tai khoan can cap nhat (Enter de huy): ");
        if (fgets(id, sizeof(id), stdin) == NULL) {
            printf("Loi khi doc ID.\n");
            return;
        }
        id[strcspn(id, "\n")] = '\0';

        if (strlen(id) == 0) {
            printf("Da huy cap nhat.\n");
            return;
        }

        found = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, id) == 0) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Khong tim thay ID '%s'. Vui long nhap lai!\n\n", id);
        } else {
            break;
        }
    }

    printf("=== Cap nhat thong tin tai khoan %s ===\n", id);

    char temp[256];

    // ===== Cap nhat Ten =====
    while (1) {
        printf("Ten hien tai: %s\n", accounts[found].fullName);
        printf("Nhap ten moi (Enter de giu nguyen): ");
        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            printf("Loi nhap ten.\n");
            return;
        }
        temp[strcspn(temp, "\n")] = '\0';

        // Neu nhap rong hoac chi khoang trang -> giu nguyen
        int hasNonSpace = 0;
        for (int i = 0; temp[i]; i++) {
            if (!isspace((unsigned char)temp[i])) {
                hasNonSpace = 1;
                break;
            }
        }
        if (!hasNonSpace) {
            break; // giu nguyen
        }

        // Kiem tra chi chu cai va khoang trang
        int valid = 1;
        for (int i = 0; temp[i]; i++) {
            if (!isalpha((unsigned char)temp[i]) && !isspace((unsigned char)temp[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("Ten chi duoc chua chu cai va khoang trang! Nhap lai.\n");
            continue;
        }

        // Hop le -> luu
        strncpy(accounts[found].fullName, temp, sizeof(accounts[found].fullName) - 1);
        accounts[found].fullName[sizeof(accounts[found].fullName) - 1] = '\0';
        break;
    }

    // ===== Cap nhat SDT =====
    while (1) {
        printf("SDT hien tai: %s\n", accounts[found].phone);
        printf("Nhap SDT moi (Enter de giu nguyen): ");
        if (fgets(temp, sizeof(temp), stdin) == NULL) {
            printf("Loi nhap SDT.\n");
            return;
        }
        temp[strcspn(temp, "\n")] = '\0';

        // Neu nhap rong hoac chi khoang trang -> giu nguyen
        int hasNonSpace = 0;
        for (int i = 0; temp[i]; i++) {
            if (!isspace((unsigned char)temp[i])) {
                hasNonSpace = 1;
                break;
            }
        }
        if (!hasNonSpace) {
            break; // giu nguyen
        }

        // Chi duoc chua so
        int valid = 1;
        for (int i = 0; temp[i]; i++) {
            if (!isdigit((unsigned char)temp[i])) {
                valid = 0;
                break;
            }
        }
        if (!valid) {
            printf("SDT chi duoc chua chu so! Nhap lai.\n");
            continue;
        }

        // Kiem tra trung
        int dup = 0;
        for (int i = 0; i < size; i++) {
            if (i != found && strcmp(accounts[i].phone, temp) == 0) {
                dup = 1;
                break;
            }
        }
        if (dup) {
            printf("SDT da ton tai! Nhap so khac.\n");
            continue;
        }

        // Hop le -> luu
        strncpy(accounts[found].phone, temp, sizeof(accounts[found].phone) - 1);
        accounts[found].phone[sizeof(accounts[found].phone) - 1] = '\0';
        break;
    }

    printf("Cap nhat thanh cong!\n");
}

void menu_f03(){ //ham hien thi menu case 3
    printf("||===============================||\n");
    printf("|| a. Khoa / Mo khoa tai khoan   ||\n");
    printf("||===============================||\n");
    printf("|| b. Xoa tai khoan              ||\n");
    printf("||===============================||\n");
}

void f03_lockOrUnlock(struct Account accounts[], int size) {
    if (size == 0) {
        printf("Chua co tai khoan nao!\n");
        return;
    }

    char id[sizeof(accounts[0].accountId)];
    int found = -1;

    // Vong lap nhap ID: không duoc rong, khong tim thay nhap lai
    while (1) {
        printf("Nhap ID tai khoan can khoa/mo: ");
        if (fgets(id, sizeof(id), stdin) == NULL) {
            printf("Loi khi doc ID.\n");
            return;
        }
        id[strcspn(id, "\n")] = '\0';

        if (strlen(id) == 0) {
            printf("ID khong duoc de trong. Vui long nhap lai!\n");
            continue; // nhap lai
        }

        found = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, id) == 0) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n", id);
            continue; // nhap lai
        }

        break; // tim thay thoat vong lap
    }

    // Hien thi trang thai hien tai
    printf("Tai khoan: %s - %s\n", accounts[found].accountId, accounts[found].fullName);
    printf("Trang thai hien tai: %s\n", accounts[found].status == 1 ? "Hoat dong" : "Khoa");

    // Xac nhan thao tac
    printf("Ban co muon %s tai khoan nay? (y/N): ", accounts[found].status == 1 ? "khoa" : "mo");
    char confirm[8];
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        printf("Loi khi doc lua chon. Huy.\n");
        return;
    }
    if (!(confirm[0] == 'y' || confirm[0] == 'Y')) {
        printf("Da huy thao tac.\n");
        return;
    }

    // Thay doi trang thai
    accounts[found].status = accounts[found].status == 1 ? 0 : 1;
    printf(">> Tai khoan '%s' da %s thanh cong!\n", accounts[found].accountId,
           accounts[found].status == 1 ? "mo" : "khoa");
}

void f03_deleteAccount(struct Account accounts[], int *size) {
    if (*size == 0) {
        printf("Chua co tai khoan nao!\n");
        return;
    }

    char id[sizeof(accounts[0].accountId)];
    int found = -1;

    // Vong lap nhap ID: không duoc rong, khong tim thay nhap lai
    while (1) {
        printf("Nhap ID tai khoan can xoa: ");
        if (fgets(id, sizeof(id), stdin) == NULL) {
            printf("Loi khi doc ID.\n");
            return;
        }
        id[strcspn(id, "\n")] = '\0';

        if (strlen(id) == 0) {
            printf("ID khong duoc de trong. Vui long nhap lai!\n");
            continue; // nhap lai
        }

        found = -1;
        for (int i = 0; i < *size; i++) {
            if (strcmp(accounts[i].accountId, id) == 0) {
                found = i;
                break;
            }
        }

        if (found == -1) {
            printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n", id);
            continue; // nhap lai
        }

        break; // tim thay thoat vong lap
    }

    // xac nhan truoc khi xoa
    printf("\nThong tin tai khoan can xoa:\n");
    printf("  ID   : %s\n", accounts[found].accountId);
    printf("  Ten  : %s\n", accounts[found].fullName);
    printf("  Phone: %s\n", accounts[found].phone);
    printf("Ban co chac chan muon xoa (y/N)? ");

    char confirm[8];
    if (fgets(confirm, sizeof(confirm), stdin) == NULL) {
        printf("Loi khi doc lua chon. Huy.\n");
        return;
    }
    if (!(confirm[0] == 'y' || confirm[0] == 'Y')) {
        printf("Da huy thao tac xoa.\n");
        return;
    }

    // Dich mang sang trai de xoa phan tu
    for (int i = found; i < *size - 1; i++) {
        accounts[i] = accounts[i + 1];
    }

    // Reset phan tu cuoi
    int last = *size - 1;
    accounts[last].accountId[0] = '\0';
    accounts[last].fullName[0] = '\0';
    accounts[last].phone[0] = '\0';
    accounts[last].balance = 0.0;
    accounts[last].status = 0;

    (*size)--;
    printf(">> Da xoa tai khoan '%s' thanh cong! Tong so hien tai: %d\n", id, *size);
}

void f04_searchAccount(struct Account accounts[], int size) {
    if (size == 0) {
        printf("Chua co tai khoan nao trong he thong!\n");
        return;
    }

    char keyword[100];
    printf("Nhap tu khoa tim kiem (ID hoac Ten): ");
    if (fgets(keyword, sizeof(keyword), stdin) == NULL) {
        printf("Loi khi doc tu khoa.\n");
        return;
    }
    keyword[strcspn(keyword, "\n")] = '\0';

    if (strlen(keyword) == 0) {
        printf("Tu khoa rong. Vui long nhap lai.\n");
        return;
    }

    /* t?o phiên b?n thu?ng hóa c?a t? khóa */
    char keyLower[sizeof(keyword)];
    size_t i;
    for (i = 0; i < sizeof(keyLower) - 1 && keyword[i] != '\0'; ++i) {
        keyLower[i] = (char)tolower((unsigned char)keyword[i]);
    }
    keyLower[i] = '\0';

    int found = 0;
    int printedIndex = 0;

    /* duy?t t?ng account, n?u kh?p thì in (và in header m?t l?n) */
    for (int idx = 0; idx < size; ++idx) {
        /* chu?n b? b?n thu?ng hóa cho ID */
        char idLower[sizeof(accounts[idx].accountId)];
        for (i = 0; i < sizeof(idLower) - 1 && accounts[idx].accountId[i] != '\0'; ++i) {
            idLower[i] = (char)tolower((unsigned char)accounts[idx].accountId[i]);
        }
        idLower[i] = '\0';

        /* chu?n b? b?n thu?ng hóa cho fullName */
        char nameLower[sizeof(accounts[idx].fullName)];
        for (i = 0; i < sizeof(nameLower) - 1 && accounts[idx].fullName[i] != '\0'; ++i) {
            nameLower[i] = (char)tolower((unsigned char)accounts[idx].fullName[i]);
        }
        nameLower[i] = '\0';

        if (strstr(idLower, keyLower) != NULL || strstr(nameLower, keyLower) != NULL) {
            if (!found) {
                /* in header ch? 1 l?n khi g?p k?t qu? d?u */
                printf("\n--- Ket qua tim kiem cho \"%s\" ---\n", keyword);
                printf("----------------------------------------------------------------------------------------------\n");
                printf("| %-3s | %-15s | %-25s | %-12s | %-10s | %-10s |\n",
                       "STT", "ID", "Ho ten", "Phone", "So du", "Trang thai");
                printf("----------------------------------------------------------------------------------------------\n");
            }

            /* in tài kho?n kh?p */
            ++printedIndex;
            printf("| %-3d | %-15s | %-25s | %-12s | %-10.2f | %-10s |\n",
                   printedIndex,
                   accounts[idx].accountId,
                   accounts[idx].fullName,
                   accounts[idx].phone,
                   accounts[idx].balance,
                   accounts[idx].status == 1 ? "Hoat dong" : "Khoa");

            found = 1;
        }
    }

    if (found) {
        printf("----------------------------------------------------------------------------------------------\n");
    } else {
        printf("Khong co ket qua phu hop.\n");
    }
}

void trim(char *str) {
// Xóa kho?ng tr?ng d?u
int start = 0;
while (isspace((unsigned char)str[start])) start++;
if (start > 0) memmove(str, str + start, strlen(str + start) + 1);

// Xóa kho?ng tr?ng cu?i
int end = strlen(str) - 1;
while (end >= 0 && isspace((unsigned char)str[end])) {
    str[end] = '\0';
    end--;
}

}

void f05_listAccounts(struct Account accounts[], int size) {
if (size == 0) {
printf("Chua co tai khoan nao trong he thong!\n");
return;
}

int pageSize;
char temp[20];
long tempLong;
char *endptr;

// ===============================
// Nh?p pageSize + validation
// ===============================
while (1) {
    printf("Nhap so luong phan tu tren moi trang (page_size > 0): ");

    if (fgets(temp, sizeof(temp), stdin) == NULL) continue;
    temp[strcspn(temp, "\n")] = '\0';

    if (strlen(temp) == 0) {
        printf("Khong duoc de trong!\n");
        continue;
    }

    tempLong = strtol(temp, &endptr, 10);
    if (*endptr != '\0') {
        printf("Gia tri khong hop le! Vui long nhap so.\n");
        continue;
    }

    if (tempLong <= 0) {
        printf("page_size phai > 0!\n");
        continue;
    }

    pageSize = (int)tempLong;
    break;
}

int totalPages = (size + pageSize - 1) / pageSize;
int currentPage = 0;
char input[20];

// ===============================
// Vòng l?p phân trang
// ===============================
while (1) {
    int start = currentPage * pageSize;
    int end = start + pageSize;
    if (end > size) end = size;

    printf("\n--- Danh sach tai khoan (Trang %d/%d) ---\n", currentPage + 1, totalPages);
    printf("----------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-15s | %-25s | %-12s | %-10s | %-10s |\n",
           "STT", "ID", "Ho ten", "Phone", "So du", "Trang thai");
    printf("----------------------------------------------------------------------------------------------\n");

    for (int i = start; i < end; i++) {
        trim(accounts[i].fullName);  // Lo?i b? kho?ng tr?ng tru?c khi hi?n th?
        printf("| %-3d | %-15s | %-25s | %-12s | %-10.2f | %-10s |\n",
               i + 1,
               accounts[i].accountId,
               accounts[i].fullName,
               accounts[i].phone,
               accounts[i].balance,
               accounts[i].status == 1 ? "Hoat dong" : "Khoa");
    }
    printf("----------------------------------------------------------------------------------------------\n");

    // ===============================
    // Nh?p di?u hu?ng trang
    // ===============================
    while (1) {
        printf("||     'n' trang sau   ||   'p' trang truoc   ||   'q' thoat   ||   So trang de nhay den     ||\n");
        printf("----------------------------------------------------------------------------------------------\n");
        printf("Nhap lua chon: ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            printf("Loi khi doc du lieu. Huy.\n");
            return;
        }
        input[strcspn(input, "\n")] = '\0';

        // X? lý l?nh n/p/q
        if (strlen(input) == 1) {
            char c = input[0];
            if (c == 'n' || c == 'N') {
                if (currentPage + 1 < totalPages) currentPage++;
                else printf("Ban dang o trang cuoi!\n");
                break;
            }
            else if (c == 'p' || c == 'P') {
                if (currentPage > 0) currentPage--;
                else printf("Ban dang o trang dau!\n");
                break;
            }
            else if (c == 'q' || c == 'Q') {
                return;
            }
        }

        // X? lý nh?p s? trang
        char *endptrPage;
        long pageLong = strtol(input, &endptrPage, 10);
        if (*endptrPage != '\0') {
            printf("Nhap khong hop le! Vui long nhap so trang.\n");
            continue;
        }
        if (pageLong <= 0 || pageLong > totalPages) {
            printf("Trang khong ton tai!\n");
            continue;
        }

        currentPage = (int)pageLong - 1;
        break;
    }
}

}

void f06_sortAccounts(struct Account accounts[], int size) {
    if (size == 0) {
        printf("Chua co tai khoan nao trong he thong!\n");
        return;
    }

    char input[20];
    long choice;
    char *endptr;

    // ============================
    // Menu lua chon
    // ============================
    while (1) {
        printf("\n||======= SAP XEP TAI KHOAN =======||\n");
        printf("||[1] Sap xep theo SO DU (Giam dan)||\n");
        printf("||=================================||\n");
        printf("||[2] Sap xep theo TEN (A -> Z)    ||\n");
        printf("||=================================||\n");
        printf("Chon (1-2): ");

        if (fgets(input, sizeof(input), stdin) == NULL) continue;
        input[strcspn(input, "\n")] = '\0';

        choice = strtol(input, &endptr, 10);

        if (*endptr == '\0' && (choice == 1 || choice == 2)) break;

        printf("Lua chon khong hop le! Vui long nhap 1 hoac 2.\n");
    }

    // ============================
    // S?p x?p
    // ============================
    for (int i = 0; i < size - 1; i++) {
        for (int j = 0; j < size - i - 1; j++) {

            int swap = 0;

            if (choice == 1) {
                // ============================
                // (1) So du giam dan
                // ============================
                if (accounts[j].balance < accounts[j + 1].balance) {
                    swap = 1;
                }
            } 
            else if (choice == 2) {
                // ============================
                // (2) Tên A - Z
                // ============================
                int cmp = strcasecmp(accounts[j].fullName, accounts[j + 1].fullName);

                if (cmp > 0) swap = 1;
                else if (cmp == 0) {
                    // N?u tên b?ng nhau ? s?p theo accountId
                    if (strcasecmp(accounts[j].accountId, accounts[j + 1].accountId) > 0)
                        swap = 1;
                }
            }

            if (swap) {
                struct Account tmp = accounts[j];
                accounts[j] = accounts[j + 1];
                accounts[j + 1] = tmp;
            }
        }
    }

    if (choice == 1)
        printf("Da sap xep theo SO DU (Giam dan)!\n");
    else
        printf("Da sap xep theo TEN (A -> Z)!\n");
}

int main() {
    struct Account accounts[100];
    int choice, size = 0;
    char input[50];

    while (1) {

        // =============================
        // NHAP LUA CHON MENU
        // =============================
        while (1) {
            displayMenu();
            printf("Moi ban nhap lua chon: ");

            if (fgets(input, sizeof(input), stdin) == NULL) {
                printf("Loi khi nhap! Hay thu lai.\n");
                continue;
            }

            input[strcspn(input, "\n")] = '\0';

            if (strlen(input) == 0) {
                printf("Khong duoc de trong! Hay nhap lai.\n");
                continue;
            }

            int hasNonSpace = 0;
            for (int i = 0; input[i]; i++) {
                if (!isspace((unsigned char)input[i])) {
                    hasNonSpace = 1;
                    break;
                }
            }
            if (!hasNonSpace) {
                printf("Khong duoc chi nhap khoang trang!\n");
                continue;
            }

            int isNumber = 1;
            for (int i = 0; input[i]; i++) {
                if (!isdigit((unsigned char)input[i])) {
                    isNumber = 0;
                    break;
                }
            }
            if (!isNumber) {
                printf("Lua chon chi duoc chua chu so!\n");
                continue;
            }

            choice = atoi(input);
            break;  // NHAP LUA CHON HOP LE -> THOAT VONG NHAP
        }

        // =============================
        // XU LY LUA CHON
        // =============================
        switch (choice) {

            case 1:
                f01_addAccount(accounts, &size);
                break;

            case 2:
                f02_updateAccount(accounts, size);
                break;

            case 3: {
                char choice_01;

                while (1) {
                    menu_f03();
                    printf("Moi ban nhap lua chon (a/b) (hoac q de thoat): ");

                    if (scanf(" %c", &choice_01) != 1) {
                        clear_stdin_line();
                        printf("Lua chon khong hop le.\n");
                        continue;
                    }
                    clear_stdin_line();

                    if (choice_01 == 'a' || choice_01 == 'A') {
                        f03_lockOrUnlock(accounts, size);
                        break;
                    }
                    else if (choice_01 == 'b' || choice_01 == 'B') {
                        f03_deleteAccount(accounts, &size);
                        break;
                    }
                    else if (choice_01 == 'q' || choice_01 == 'Q') {
                        break;  // THOAT MENU 3 -> VE MENU CHINH
                    }
                    else {
                        printf("Lua chon khong hop le! Hay nhap a/b (hoac q de thoat).\n");
                    }
                }
                break;
            }

            case 4:
                f04_searchAccount(accounts, size);
                break;

            case 5:
                f05_listAccounts(accounts, size);
                break;

            case 6:
                f06_sortAccounts(accounts, size);
                break;
                
            case 7: 
				
				break;	
            case 9:
                printf("Thoat chuong trinh.\n");
                return 0;

            default:
                printf("Khong ton tai chuc nang nay!\n");
                break;
        }

    } 

    return 0;
}
