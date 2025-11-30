#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>   /* <-- Thêm d? l?y th?i gian cho giao d?ch */

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

/* =========================
   Hàm ti?n ích dã tách ra
   ========================= */

/* Ð?c m?t dòng vào buffer (fgets + strip newline). Tr? v? 1 khi thành công, 0 khi EOF */
int read_line(char *buffer, size_t size) {
    if (fgets(buffer, (int)size, stdin) == NULL) return 0;
    buffer[strcspn(buffer, "\n")] = '\0';
    return 1;
}

/* Ki?m tra chu?i ch? ch?a kho?ng tr?ng hay r?ng -> tr? v? 1 n?u có ch? kho?ng tr?ng (không h?p l?) */
int is_only_spaces(const char *s) {
    for (int i = 0; s[i]; i++) {
        if (!isspace((unsigned char)s[i])) return 0;
    }
    return 1;
}

/* Ki?m tra chu?i ch? g?m ch? cái và kho?ng tr?ng */
int isLetterString(const char *str){
    for(int i = 0; str[i] != '\0'; i++){
        if(!( (str[i] >= 'A' && str[i] <= 'Z') ||
              (str[i] >= 'a' && str[i] <= 'z') ||
               str[i] == ' ' )) {
            return 0; // có ký t? không h?p l?
        }
    }
    return 1; // h?p l?
}

/* Trim in-place (xóa kho?ng tr?ng d?u và cu?i) */
void trim(char *str) {
    int start = 0;
    while (isspace((unsigned char)str[start])) start++;
    if (start > 0) memmove(str, str + start, strlen(str + start) + 1);

    int end = (int)strlen(str) - 1;
    while (end >= 0 && isspace((unsigned char)str[end])) {
        str[end] = '\0';
        end--;
    }
}

/* Sao chép chu?i src sang dst v?i chuy?n sang ch? thu?ng; dstSize là kích thu?c buffer dích */
void to_lower_copy(const char *src, char *dst, size_t dstSize) {
    size_t i;
    for (i = 0; i < dstSize - 1 && src[i] != '\0'; ++i) {
        dst[i] = (char)tolower((unsigned char)src[i]);
    }
    dst[i] = '\0';
}

/* Ð?c 1 chu?i không du?c r?ng (không nh?n Enter tr?ng ho?c ch? kho?ng tr?ng) */
int read_nonempty_string(const char *prompt, char *out, size_t outSize) {
    while (1) {
        printf("%s", prompt);
        if (!read_line(out, outSize)) return 0;
        if (strlen(out) == 0) {
            printf("Khong duoc de trong! Hay nhap lai.\n");
            continue;
        }
        if (is_only_spaces(out)) {
            printf("Khong duoc chi nhap khoang trang! Hay nhap lai.\n");
            continue;
        }
        return 1;
    }
}

/* Ð?c s? nguyên duong v?i gi?i h?n minVal..maxVal */
int read_positive_int(const char *prompt, int *out, int minVal, int maxVal) {
    char temp[64];
    char *endptr;
    long val;
    while (1) {
        printf("%s", prompt);
        if (!read_line(temp, sizeof(temp))) return 0;
        if (strlen(temp) == 0) {
            printf("Khong duoc de trong!\n");
            continue;
        }
        if (is_only_spaces(temp)) {
            printf("Khong duoc chi nhap khoang trang! Hay nhap lai.\n");
            continue;
        }
        int isNumber = 1;
        for (int i = 0; temp[i]; i++) {
            if (!isdigit((unsigned char)temp[i])) {
                isNumber = 0;
                break;
            }
        }
        if (!isNumber) {
            printf("Chi duoc nhap so! Hay nhap lai.\n");
            continue;
        }
        val = strtol(temp, &endptr, 10);
        if (*endptr != '\0') {
            printf("Gia tri khong hop le! Vui long nhap so.\n");
            continue;
        }
        if (val < minVal || val > maxVal) {
            if (minVal == 1 && maxVal == 100)
                printf("So luong khong hop le! (1..100)\n");
            else
                printf("Gia tri khong hop le! (Phai trong khoang %d..%d)\n", minVal, maxVal);
            continue;
        }
        *out = (int)val;
        return 1;
    }
}

/* Ð?c và validate ID (không r?ng, ch? alnum, không trùng theo accountsUsed) */
int read_valid_id(const char *prompt, char *out, size_t outSize, struct Account accounts[], int accountsUsed) {
    char tmp[64];
    while (1) {
        printf("%s", prompt);
        if (!read_line(tmp, sizeof(tmp))) return 0;
        if (strlen(tmp) == 0) {
            printf("ID khong duoc de trong! Vui long nhap lai.\n");
            continue;
        }
        if (is_only_spaces(tmp)) {
            printf("ID khong duoc chi co khoang trang! Hay nhap lai.\n");
            continue;
        }
        int hasInvalidChar = 0;
        for (size_t p = 0; p < strlen(tmp); p++) {
            if (!isalnum((unsigned char)tmp[p])) {
                hasInvalidChar = 1;
                break;
            }
        }
        if (hasInvalidChar) {
            printf("ID chi duoc chua chu va so! Khong duoc chua ky tu dac biet.\n");
            continue;
        }

        int isDuplicate = 0;
        for (int j = 0; j < accountsUsed; j++) {
            if (strcmp(tmp, accounts[j].accountId) == 0) {
                isDuplicate = 1;
                break;
            }
        }
        if (isDuplicate) {
            printf(">> Loi: ID nay da ton tai! Hay nhap ID khac.\n");
            continue;
        }

        strncpy(out, tmp, outSize - 1);
        out[outSize - 1] = '\0';
        return 1;
    }
}

/* Ð?c và validate fullName */
int read_valid_fullname(const char *prompt, char *out, size_t outSize) {
    while (1) {
        printf("%s", prompt);
        if (!read_line(out, outSize)) return 0;
        if (strlen(out) == 0) {
            printf("Ten khong duoc de trong!\n");
            continue;
        }
        int hasLetter = 0;
        for (int i = 0; out[i]; i++) {
            if (isalpha((unsigned char)out[i])) {
                hasLetter = 1;
                break;
            }
        }
        if (!hasLetter) {
            printf("Ten khong duoc chi co khoang trang! Hay nhap lai.\n");
            continue;
        }
        if (!isLetterString(out)) {
            printf("Ten chi duoc chua chu cai va khoang trang!\n");
            continue;
        }
        return 1;
    }
}

/* Ð?c và validate phone */
int read_valid_phone(const char *prompt, char *out, size_t outSize, struct Account accounts[], int accountsUsed) {
    char tmp[64];
    while (1) {
        printf("%s", prompt);
        if (!read_line(tmp, sizeof(tmp))) return 0;
        if (strlen(tmp) == 0) {
            printf("So dien thoai khong duoc de trong!\n");
            continue;
        }
        if (is_only_spaces(tmp)) {
            printf("So dien thoai khong duoc chi co khoang trang!\n");
            continue;
        }
        int hasNonDigit = 0;
        for (size_t p = 0; p < strlen(tmp); p++) {
            if (!isdigit((unsigned char)tmp[p])) {
                hasNonDigit = 1;
                break;
            }
        }
        if (hasNonDigit) {
            printf("So dien thoai chi duoc chua chu so!\n");
            continue;
        }
        int duplicated = 0;
        for (int j = 0; j < accountsUsed; j++) {
            if (strcmp(accounts[j].phone, tmp) == 0) {
                duplicated = 1;
                break;
            }
        }
        if (duplicated) {
            printf("So dien thoai da ton tai, vui long nhap so khac!\n");
            continue;
        }
        strncpy(out, tmp, outSize - 1);
        out[outSize - 1] = '\0';
        return 1;
    }
}

/* Ð?c ID d? UPDATE / LOCK / DELETE: yêu c?u không du?c r?ng, n?u không tìm th?y -> nh?p l?i.
   Tr? v? index tìm th?y, -1 n?u EOF */
int read_existing_id_index(const char *prompt, struct Account accounts[], int size) {
    char id[64];
    while (1) {
        printf("%s", prompt);
        if (!read_line(id, sizeof(id))) { printf("Loi khi doc ID.\n"); return -1; }
        if (strlen(id) == 0) {
            printf("ID khong duoc de trong. Vui long nhap lai!\n");
            continue;
        }
        int found = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, id) == 0) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n", id);
            continue;
        }
        return found;
    }
}

/* Ð?c ID khi c?p nh?t (cho phép Enter d? h?y): tr? v? -2 n?u h?y, -1 n?u error, >=0 là index tìm th?y */
int read_existing_id_index_allow_cancel(const char *prompt, struct Account accounts[], int size) {
    char id[64];
    while (1) {
        printf("%s", prompt);
        if (!read_line(id, sizeof(id))) { printf("Loi khi doc ID.\n"); return -1; }
        if (strlen(id) == 0) {
            printf("Da huy cap nhat.\n");
            return -2; /* cancel */
        }
        int found = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, id) == 0) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            printf("Khong tim thay ID '%s'. Vui long nhap lai!\n\n", id);
            continue;
        }
        return found;
    }
}

/* Ð?c chu?i d? update: n?u Enter ho?c r?ng -> caller gi? nguyên */
int read_update_field(char *out, size_t outSize) {
    if (!read_line(out, outSize)) return 0;
    return 1;
}

/* Ð?c l?a ch?n confirm y/N */
int read_confirm_yes(void) {
    char confirm[8];
    if (!read_line(confirm, sizeof(confirm))) { printf("Loi khi doc lua chon. Huy.\n"); return -1; }
    if (confirm[0] == 'y' || confirm[0] == 'Y') return 1;
    return 0;
}

/* =========================
   Các hàm chính (s? d?ng helper)
   ========================= */

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

    // ================================================================
    // VONG LAP KIEM TRA DAU VAO (VALIDATION LOOP)
    // ================================================================
    while (1) {
        printf("Nhap tu khoa tim kiem (ID hoac Ten): ");
        
        if (fgets(keyword, sizeof(keyword), stdin) == NULL) {
            continue; 
        }
        
        // Xoa ky tu xuong dong
        keyword[strcspn(keyword, "\n")] = '\0';

        // 1. CHAN NHAP RONG (ENTER)
        if (strlen(keyword) == 0) {
            printf(">> Loi: Khong duoc de trong! Vui long nhap lai.\n");
            continue; 
        }

        // 2. CHAN NHAP TOAN KHOANG TRANG
        int isAllSpace = 1;
        for (int i = 0; i < strlen(keyword); i++) {
            if (keyword[i] != ' ') {
                isAllSpace = 0;
                break;
            }
        }
        if (isAllSpace) {
             printf(">> Loi: Khong duoc chi nhap khoang trang! Vui long nhap lai.\n");
             continue;
        }

        // 3. CHAN KY TU DAC BIET (Bao gom ca dau '-' neu co)
        int hasInvalidChar = 0;
        for (int i = 0; i < strlen(keyword); i++) {
            if (!isalnum((unsigned char)keyword[i]) && keyword[i] != ' ') {
                hasInvalidChar = 1;
                break;
            }
        }
        if (hasInvalidChar) {
            printf(">> Loi: Chua ky tu dac biet khong hop le! Chi duoc nhap Chu va So.\n");
            continue;
        }

        // Input hop le -> Thoat vong lap
        break; 
    }
    // ================================================================


    // --- PHAN TIM KIEM (GIU NGUYEN) ---
    char keyLower[100];
    size_t i;
    for (i = 0; i < sizeof(keyLower) - 1 && keyword[i] != '\0'; ++i) {
        keyLower[i] = (char)tolower((unsigned char)keyword[i]);
    }
    keyLower[i] = '\0';

    int found = 0;
    int printedIndex = 0;

    for (int idx = 0; idx < size; ++idx) {
        char idLower[20], nameLower[50];
        int j;

        for (j = 0; accounts[idx].accountId[j] != '\0'; j++) 
            idLower[j] = (char)tolower((unsigned char)accounts[idx].accountId[j]);
        idLower[j] = '\0';

        for (j = 0; accounts[idx].fullName[j] != '\0'; j++) 
            nameLower[j] = (char)tolower((unsigned char)accounts[idx].fullName[j]);
        nameLower[j] = '\0';

        if (strstr(idLower, keyLower) != NULL || strstr(nameLower, keyLower) != NULL) {
            if (!found) {
                printf("\n--- Ket qua tim kiem cho \"%s\" ---\n", keyword);
                printf("----------------------------------------------------------------------------------------------\n");
                printf("| %-3s | %-15s | %-25s | %-12s | %-10s | %-10s |\n", "STT", "ID", "Ho ten", "Phone", "So du", "Trang thai");
                printf("----------------------------------------------------------------------------------------------\n");
            }
            ++printedIndex;
            printf("| %-3d | %-15s | %-25s | %-12s | %-10.2f | %-10s |\n",
                   printedIndex, accounts[idx].accountId, accounts[idx].fullName, accounts[idx].phone, accounts[idx].balance,
                   accounts[idx].status == 1 ? "Hoat dong" : "Khoa");
            found = 1;
        }
    }

    if (found) {
        printf("----------------------------------------------------------------------------------------------\n");
    } else {
        printf(">> Khong tim thay ket qua nao phu hop.\n");
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
            trim(accounts[i].fullName);  // Loai bo khoang trang truoc khi hien thi
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
                    // Neu ten bang nhau -> sap theo accountId
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

/* Case 7: Chuyen khoan */
void f07_transfer(struct Account accounts[], int size, struct Transaction transactions[], int *transCount) {
    if (size == 0) {
        printf("Chua co tai khoan nao trong he thong!\n");
        return;
    }

    char sendId[20], recvId[20], buf[64];
    int senderIdx = -1, receiverIdx = -1;

    // ===== NHAP ID NGUOI GUI =====
    while (1) {
        printf("Nhap ID nguoi gui: ");
        if (!read_line(sendId, sizeof(sendId))) { 
            printf("Loi khi doc du lieu.\n"); 
            continue; 
        }
        if (strlen(sendId) == 0) {
            printf("ID khong duoc de trong! Hay nhap lai.\n");
            continue;
        }

        senderIdx = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, sendId) == 0) {
                senderIdx = i;
                break;
            }
        }

        if (senderIdx == -1) {
            printf("Khong tim thay tai khoan nguoi gui! Hay nhap lai.\n");
            continue;
        }

        if (accounts[senderIdx].status == 0) {
            printf("Tai khoan nguoi gui dang bi KHOA! Vui long nhap tai khoan khac.\n");
            continue;   // <<=== CH? NH?P L?I, KHÔNG QUAY L?I MENU
        }

        break;  // ngu?i g?i h?p l?
    }


    // ===== NHAP ID NGUOI NHAN =====
    while (1) {
        printf("Nhap ID nguoi nhan: ");
        if (!read_line(recvId, sizeof(recvId))) { 
            printf("Loi khi doc du lieu.\n"); 
            continue; 
        }
        if (strlen(recvId) == 0) {
            printf("ID khong duoc de trong! Hay nhap lai.\n");
            continue;
        }

        receiverIdx = -1;
        for (int i = 0; i < size; i++) {
            if (strcmp(accounts[i].accountId, recvId) == 0) {
                receiverIdx = i;
                break;
            }
        }

        if (receiverIdx == -1) {
            printf("Khong tim thay tai khoan nguoi nhan! Hay nhap lai.\n");
            continue;
        }

        if (receiverIdx == senderIdx) {
            printf("Nguoi nhan phai khac nguoi gui! Nhap lai.\n");
            continue;
        }

        if (accounts[receiverIdx].status == 0) {
            printf("Tai khoan nguoi nhan dang bi KHOA! Vui long nhap tai khoan khac.\n");
            continue;  // <<=== CH? NH?P L?I, KHÔNG THOÁT
        }

        break; // ngu?i nh?n h?p l?
    }


    // ===== NHAP SO TIEN =====
    double amount = 0.0;

    while (1) {
        printf("Nhap so tien can chuyen: ");
        if (!read_line(buf, sizeof(buf))) { 
            printf("Loi khi doc so tien.\n"); 
            continue; 
        }

        if (strlen(buf) == 0) {
            printf("So tien khong duoc de trong! Nhap lai.\n");
            continue;
        }

        char *endptr;
        amount = strtod(buf, &endptr);

        if (*endptr != '\0') {
            printf("Gia tri khong hop le! Chi duoc nhap so.\n");
            continue;
        }

        if (amount <= 0) {
            printf("So tien phai lon hon 0! Nhap lai.\n");
            continue;
        }

        if (amount > accounts[senderIdx].balance) {
            printf("So du khong du! Hien co: %.2f\n", accounts[senderIdx].balance);
            continue;
        }

        break;
    }


    // ===== THUC HIEN CHUYEN KHOAN =====
    accounts[senderIdx].balance -= amount;
    accounts[receiverIdx].balance += amount;

    // Luu lich su giao dich
    int idx = *transCount;
    snprintf(transactions[idx].transId, sizeof(transactions[idx].transId), "T%04d", idx + 1);

    strcpy(transactions[idx].senderId, accounts[senderIdx].accountId);
    strcpy(transactions[idx].receiverId, accounts[receiverIdx].accountId);
    transactions[idx].amount = amount;
    strcpy(transactions[idx].type, "TRF");

    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(transactions[idx].date, sizeof(transactions[idx].date),
             "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);

    (*transCount)++;

    printf("\n>> Chuyen tien thanh cong!\n");
    printf("Ma giao dich: %s\n", transactions[idx].transId);
    printf("So du moi cua nguoi gui: %.2f\n", accounts[senderIdx].balance);
}

void f08_showTransactions(struct Transaction transactions[], int transCount) {
    if (transCount == 0) {
        printf("Chua co lich su giao dich nao!\n");
        return;
    }

    printf("\n--- Lich su giao dich (Tong: %d) ---\n", transCount);
    printf("------------------------------------------------------------------------------------------\n");
    printf("| %-3s | %-8s | %-12s | %-12s | %-12s | %-6s | %-19s |\n", "STT", "TransID", "SenderID", "ReceiverID", "Amount", "Type", "Date");
    printf("------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < transCount; i++) {
        printf("| %-3d | %-8s | %-12s | %-12s | %12.2f | %-6s | %-19s |\n",
               i + 1,
               transactions[i].transId,
               transactions[i].senderId,
               transactions[i].receiverId,
               transactions[i].amount,
               transactions[i].type,
               transactions[i].date);
    }
    printf("------------------------------------------------------------------------------------------\n");
}

int main() {
    struct Account accounts[100];
    struct Transaction transactions[1000]; /* luu t?i da 1000 giao d?ch */
    int transCount = 0;

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
                f07_transfer(accounts, size, transactions, &transCount);
                break;

            case 8:
                f08_showTransactions(transactions, transCount);
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
