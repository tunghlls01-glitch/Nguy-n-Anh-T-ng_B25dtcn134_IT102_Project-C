#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>

#define MAX_ACCOUNTS 100
#define MAX_TX 1000

struct Account {
    char accountId[20];
    char fullName[50];
    char phone[15];
    double balance;
    int status; // 1 active, 0 locked
};

struct Transaction {
    char transId[20];
    char senderId[20];
    char receiverId[20];
    double amount;
    char type[10];
    char date[20];
};


int read_line(char *buf, size_t n) {
    if (!fgets(buf, (int)n, stdin)){
        return 0;
    }
    buf[strcspn(buf, "\n")] = '\0';
    return 1;
}

int is_only_spaces(const char *s) {
    if (s[0] == '\0') return 0;
    for (int i = 0; s[i]; i++) {
        if (!isspace((unsigned char)s[i])) {
            return 0;
        }
    }
    return 1;
}

int only_digits(const char *s) {
    if (s[0] == '\0') {
    	return 0;
	}
    for (size_t i = 0; s[i]; i++) {
    	if (!isdigit((unsigned char)s[i])){
			 return 0;
		}
    }
    return 1;
}

int only_letters_spaces(const char *s) {
    if (s[0] == '\0') {
    	return 0;
	}
    for (size_t i = 0; s[i]; i++) {
    	if (!isalpha((unsigned char)s[i]) && !isspace((unsigned char)s[i])){
    		return 0;
		}
	}
    return 1;
}

int find_account_by_id(struct Account arr[], int n, const char *id) {
    for (int i = 0; i < n; ++i) {
    	if (strcmp(arr[i].accountId, id) == 0) {
			return i;
		}
	}
    return -1;
}

void now_str(char *out, size_t n) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    snprintf(out, n, "%04d-%02d-%02d %02d:%02d:%02d",
             tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
}

void displayMenu (){ // ham hien thi menu
    printf("\n||=========Mini Banking System========||\n");
    printf("||      1. Them tai khoan moi         ||\n");
    printf("||      2. Cap nhat thong tin         ||\n");
    printf("||      3. Quan ly trang thai         ||\n");
    printf("||      4. Tra cuu(tim kiem)          ||\n");
    printf("||      5. Danh sach                  ||\n");
    printf("||      6. Sap xep danh sach          ||\n");
    printf("||      7. Giao dich chuyen khoan     ||\n");
    printf("||      8. Lich su giao dich          ||\n");
    printf("||      9. Thoat                      ||\n");
    printf("||====================================||\n");
}

void clear_stdin_line(void) { //ham doc va xoa ky tu '\n' con sot lai tu sanf
    int c;
    while((c = getchar()) != '\n' && c != EOF) {
	}
}

// ==========================================================
// HAM MOI: XOA KHOANG TRANG O DAU VA CUOI CHUOI
// ==========================================================
void trim_whitespace(char *s) {
    if (s == NULL) return;

    // 1. Xoa khoang trang o cuoi chuoi
    int i = (int)strlen(s) - 1;
    while (i >= 0 && isspace((unsigned char)s[i])) {
        s[i] = '\0';
        i--;
    }

    // 2. Xoa khoang trang o dau chuoi
    int start = 0;
    while (isspace((unsigned char)s[start])) {
        start++;
    }

    // Neu co khoang trang o dau, dich chuyen chuoi ve truoc
    if (start > 0) {
        int j = 0;
        while (s[start]) {
            s[j++] = s[start++];
        }
        s[j] = '\0';
    }
}
// ==========================================================

int prompt_alnum_unique(const char *prompt, char *out, size_t n, struct Account accounts[], int used) { // ham vong lap dung cho ID va ma tai khoan,f01
    char tmp[64];
    while (1) {
        printf("%s", prompt);
        if (!read_line(tmp, sizeof(tmp))) {
        		return 0;
		}
        if (strlen(tmp) == 0) {
            printf("ID khong duoc de trong! Vui long nhap lai.\n");
            continue;
        }
        if (is_only_spaces(tmp)) {
            printf("ID khong duoc chi co khoang trang! Hay nhap lai.\n");
            continue;
        }
        int bad = 0;
        for (size_t i = 0; i < strlen(tmp); ++i)
            if (!isalnum((unsigned char)tmp[i])) {
			bad = 1;
			break;
		}
        if (bad) {
			printf("ID chi duoc chua chu va so! Khong duoc chua ky tu dac biet.\n");
			continue;
		}
        if (find_account_by_id(accounts, used, tmp) != -1) {
			printf(">> Loi: ID nay da ton tai! Hay nhap ID khac.\n");
			continue;
		}
        strncpy(out, tmp, n-1); out[n-1] = '\0';
		return 1;
    }
}

// ==========================================================
// HAM DA CAP NHAT: TU DONG TRIM TRUOC KHI XAC THUC
// ==========================================================
int prompt_nonempty_name(const char *prompt, char *out, size_t n) {
    char tmp[128];
    while (1) {
        printf("%s", prompt); // "Nhap ten (FullName): "
        if (!read_line(tmp, sizeof(tmp))) return 0;
        
        // Them: Xoa khoang trang o dau/cuoi tren ban sao tam thoi (hoac ngay tren tmp)
        trim_whitespace(tmp); // <--- DUNG HAM MOI
        
        if (strlen(tmp) == 0) {
            printf("Ten khong duoc de trong (sau khi xoa khoang trang)! Hay nhap lai.\n");
            continue;
        }
        
        int hasLetter = 0;
        for (int i = 0; tmp[i]; i++) if (isalpha((unsigned char)tmp[i])) {
			hasLetter = 1;
			break;
		}
        if (!hasLetter) {
			printf("Ten khong duoc chi co khoang trang! Hay nhap lai.\n");
			continue;
		}
        if (!only_letters_spaces(tmp)) {
			printf("Ten chi duoc chua chu cai va khoang trang!\n");
			continue;
		}
        strncpy(out, tmp, n-1); out[n-1] = '\0'; return 1;
    }
}
// ==========================================================


int prompt_unique_phone(const char *prompt, char *out, size_t n, struct Account accounts[], int used) {
    char tmp[64];
    while (1) {
        printf("%s", prompt); // "Nhap so dien thoai (phone): "
        if (!read_line(tmp, sizeof(tmp))){
			return 0;
		}
        if (strlen(tmp) == 0) {
            printf("So dien thoai khong duoc de trong!\n");
            continue;
        }
        if (is_only_spaces(tmp)) {
            printf("So dien thoai khong duoc chi co khoang trang!\n");
            continue;
        }
        int hasNonDigit = 0;
        for (size_t p = 0; p < strlen(tmp); p++){
			if (!isdigit((unsigned char)tmp[p])) {
			hasNonDigit = 1;
			break;
			}
		}
        if (hasNonDigit) {
			printf("So dien thoai chi duoc chua chu so!\n");
			continue;
		}
        int dup = 0;
        for (int j = 0; j < used; j++) {
        	if (strcmp(accounts[j].phone, tmp) == 0) {
			dup = 1;
			break;
			}
		}
        if (dup) {
			printf("So dien thoai da ton tai, vui long nhap so khac!\n");
			continue;
		}
        strncpy(out, tmp, n-1); out[n-1] = '\0';
		return 1;
    }
}

int prompt_positive_double_original(const char *prompt, double *out) {
    char buf[64];
    while (1) {
        printf("%s", prompt); // "Nhap so tien can chuyen: "
        if (!read_line(buf, sizeof(buf))){
        	return 0;
		}
        if (strlen(buf) == 0) {
			printf("So tien khong duoc de trong! Nhap lai.\n");
			continue;
		}
        char *endptr;
        double v = strtod(buf, &endptr);
        if (*endptr != '\0') {
			printf("Gia tri khong hop le! Chi duoc nhap so.\n");
			continue;
		}
        if (v <= 0) {
			printf("So tien phai lon hon 0! Nhap lai.\n");
			continue;
		}
        *out = v;
		return 1;
    }
}

void f01_addAccount(struct Account accounts[], int *size) {
    int addCount;
    char input[50];

    while (1) {
        printf("Nhap so luong tai khoan: ");
        if (!read_line(input, sizeof(input))) {
			printf("Loi khi nhap! Hay thu lai.\n");
			continue;
		}
        if (strlen(input) == 0) {
			printf("Khong duoc de trong! Hay nhap lai.\n");
			continue;
		}
        if (is_only_spaces(input)) {
			printf("Khong duoc chi nhap khoang trang! Hay nhap lai.\n");
			continue;
		}
        if (!only_digits(input)) {
			printf("Chi duoc nhap so! Hay nhap lai.\n");
			continue;
		}
        addCount = atoi(input);
        if (addCount <= 0 || addCount > 100) {
			printf("So luong khong hop le! (1..100)\n");
			continue;
		}
        if (*size + addCount > 100) {
			("Khong du suc chua de them %d tai khoan. Con lai: %d\n", addCount, 100 - *size);
			continue;
		}
        break;
    }

    for (int k = 0; k < addCount; k++) {
        int idx = *size + k;
        printf("----------Tai khoan nguoi dung %d ----------\n", idx + 1);

        // ID
        while (1) {
            if (!prompt_alnum_unique("Nhap ID (ma tai khoan): ", accounts[idx].accountId, sizeof(accounts[idx].accountId), accounts, *size + k))
                strcpy(accounts[idx].accountId, "");
            else break;
        }

        // Ten (prompt_nonempty_name da tu dong trim)
        while (1) {
            if (prompt_nonempty_name("Nhap ten (FullName): ", accounts[idx].fullName, sizeof(accounts[idx].fullName))) break;
        }

        // Phone
        while (1) {
            if (prompt_unique_phone("Nhap so dien thoai (phone): ", accounts[idx].phone, sizeof(accounts[idx].phone), accounts, *size + k)) break;
        }

        accounts[idx].balance = 100000.0;
        accounts[idx].status = 1;
    }

    *size += addCount;
    printf("Them tai khoan thanh cong! Tong so hien tai: %d\n", *size);
}

// ==========================================================
// HAM DA CAP NHAT: THEM G?I HAM TRIM
// ==========================================================
void f02_updateAccount(struct Account accounts[], int size) {
    if (size == 0) {
		printf("Chua co tai khoan nao trong he thong!\n");
		return;
	}
    char id[20], temp[256];
    int found = -1;

    while (1) {
        printf("Nhap ID tai khoan can cap nhat (Enter de huy): ");
        if (!read_line(id, sizeof(id))) {
			printf("Loi khi doc ID.\n");
			return;
		}
        if (strlen(id) == 0) {
			printf("Da huy cap nhat.\n");
			return; }
        found = find_account_by_id(accounts, size, id);
        if (found == -1) {
			printf("Khong tim thay ID '%s'. Vui long nhap lai!\n\n", id);
			continue;
		}
        break;
    }

    printf("=== Cap nhat thong tin tai khoan %s ===\n", id);

    // Ten
    while (1) {
        printf("Ten hien tai: %s\n", accounts[found].fullName);
        printf("Nhap ten moi (Enter de giu nguyen): ");
        if (!read_line(temp, sizeof(temp))) {
			printf("Loi nhap ten.\n");
			return;
		}
        
        // Xóa kho?ng tr?ng ? d?u và cu?i chu?i t?m tru?c khi ki?m tra r?ng
        trim_whitespace(temp); // <--- DUNG HAM MOI
        
        if (strlen(temp) == 0){
        	break; // giu nguyen
		}
        
        if (!only_letters_spaces(temp)) {
			printf("Ten chi duoc chua chu cai va khoang trang! Nhap lai.\n");
			continue;
		}
        strncpy(accounts[found].fullName, temp, sizeof(accounts[found].fullName)-1);
        accounts[found].fullName[sizeof(accounts[found].fullName)-1] = '\0';
        break;
    }

    // SDT
    while (1) {
        printf("SDT hien tai: %s\n", accounts[found].phone);
        printf("Nhap SDT moi (Enter de giu nguyen): ");
        if (!read_line(temp, sizeof(temp))) {
			printf("Loi nhap SDT.\n");
			return;
		}
        if (strlen(temp) == 0 || is_only_spaces(temp))
			break; // giu nguyen
        if (!only_digits(temp)) {
			printf("SDT chi duoc chua chu so! Nhap lai.\n");
			continue;
		}
        int dup = 0;
        for (int i = 0; i < size; i++)
			if (i != found && strcmp(accounts[i].phone, temp) == 0) {
			dup = 1;
			break;
		}
        if (dup) {
			printf("SDT da ton tai! Nhap so khac.\n");
			continue;
		}
        strncpy(accounts[found].phone, temp, sizeof(accounts[found].phone)-1);
        accounts[found].phone[sizeof(accounts[found].phone)-1] = '\0';
        break;
    }

    printf("Cap nhat thanh cong!\n");
}
// ==========================================================

void menu_f03(){ //ham hien thi menu case 3
    printf("||===============================||\n");
    printf("|| a. Khoa / Mo khoa tai khoan   ||\n");
    printf("||===============================||\n");
    printf("|| b. Xoa tai khoan              ||\n");
    printf("||===============================||\n");
}

void f03_lockOrUnlock(struct Account accounts[], int size) {
    if (size == 0) { printf("Chua co tai khoan nao!\n"); return; }
    char id[20], confirm[8];
    int found = -1;

    while (1) {
        printf("Nhap ID tai khoan can khoa/mo: ");
        if (!read_line(id, sizeof(id))) { printf("Loi khi doc ID.\n"); return; }
        if (strlen(id) == 0) { printf("ID khong duoc de trong. Vui long nhap lai!\n"); continue; }
        found = find_account_by_id(accounts, size, id);
        if (found == -1) { printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n", id); continue; }
        break;
    }

    printf("Tai khoan: %s - %s\n", accounts[found].accountId, accounts[found].fullName);
    printf("Trang thai hien tai: %s\n", accounts[found].status == 1 ? "Hoat dong" : "Khoa");

    printf("Ban co muon %s tai khoan nay? (y/N): ", accounts[found].status == 1 ? "khoa" : "mo");
    if (!read_line(confirm, sizeof(confirm))) { printf("Loi khi doc lua chon. Huy.\n"); return; }
    if (!(confirm[0] == 'y' || confirm[0] == 'Y')) { printf("Da huy thao tac.\n"); return; }

    accounts[found].status = accounts[found].status == 1 ? 0 : 1;
    printf(">> Tai khoan '%s' da %s thanh cong!\n", accounts[found].accountId,
           accounts[found].status == 1 ? "mo" : "khoa");
}

void f03_deleteAccount(struct Account accounts[], int *size) {
    if (*size == 0) { printf("Chua co tai khoan nao!\n"); return; }
    char id[20], confirm[8];
    int found = -1;

    while (1) {
        printf("Nhap ID tai khoan can xoa: ");
        if (!read_line(id, sizeof(id))) { printf("Loi khi doc ID.\n"); return; }
        if (strlen(id) == 0) { printf("ID khong duoc de trong. Vui long nhap lai!\n"); continue; }
        found = find_account_by_id(accounts, *size, id);
        if (found == -1) { printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n", id); continue; }
        break;
    }

    printf("\nThong tin tai khoan can xoa:\n");
    printf("  ID   : %s\n", accounts[found].accountId);
    printf("  Ten  : %s\n", accounts[found].fullName);
    printf("  Phone: %s\n", accounts[found].phone);
    printf("Ban co chac chan muon xoa (y/N)? ");

    if (!read_line(confirm, sizeof(confirm))) {
		printf("Loi khi doc lua chon. Huy.\n");
		return;
	}
    if (!(confirm[0] == 'y' || confirm[0] == 'Y')) {
		printf("Da huy thao tac xoa.\n");
		return;
	}

    for (int i = found; i < *size - 1; i++) accounts[i] = accounts[i + 1];

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

    while (1) {
        printf("Nhap tu khoa tim kiem (ID hoac Ten): ");
        if (!read_line(keyword, sizeof(keyword)))
			continue;
        if (strlen(keyword) == 0) {
			printf(">> Loi: Khong duoc de trong! Vui long nhap lai.\n");
			continue;
		}
        if (is_only_spaces(keyword)) {
			printf(">> Loi: Khong duoc chi nhap khoang trang! Vui long nhap lai.\n");
			continue;
		}
        int hasInvalidChar = 0;
        for (int i = 0; keyword[i]; i++)
			if (!isalnum((unsigned char)keyword[i]) && keyword[i] != ' ') {
				hasInvalidChar = 1;
				break;
			}
        if (hasInvalidChar) {
			printf(">> Loi: Chua ky tu dac biet khong hop le! Chi duoc nhap Chu va So.\n");
			continue;
		}
        break;
    }

    char keyLower[100];
    size_t i;
    for (i = 0; i < sizeof(keyLower) - 1 && keyword[i] != '\0'; ++i) keyLower[i] = (char)tolower((unsigned char)keyword[i]);
    keyLower[i] = '\0';

    int found = 0, printedIndex = 0;
    for (int idx = 0; idx < size; ++idx) {
        char idLower[20], nameLower[50];
        int j;
        for (j = 0; accounts[idx].accountId[j] != '\0'; j++) idLower[j] = (char)tolower((unsigned char)accounts[idx].accountId[j]); idLower[j] = '\0';
        for (j = 0; accounts[idx].fullName[j] != '\0'; j++) nameLower[j] = (char)tolower((unsigned char)accounts[idx].fullName[j]); nameLower[j] = '\0';

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

    if (found) printf("----------------------------------------------------------------------------------------------\n");
    else printf(">> Khong tim thay ket qua nao phu hop.\n");
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

    while (1) {
        printf("Nhap so luong phan tu tren moi trang (page_size > 0): ");
        if (!read_line(temp, sizeof(temp)))
			continue;
        if (strlen(temp) == 0) { printf("Khong duoc de trong!\n");
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
        pageSize = (int)tempLong; break;
    }

    int totalPages = (size + pageSize - 1) / pageSize;
    int currentPage = 0;
    char input[20];

    while (1) {
        int start = currentPage * pageSize;
        int end = start + pageSize; if (end > size) end = size;
        printf("\n--- Danh sach tai khoan (Trang %d/%d) ---\n", currentPage + 1, totalPages);
        printf("----------------------------------------------------------------------------------------------\n");
        printf("| %-3s | %-15s | %-25s | %-12s | %-10s | %-10s |\n", "STT", "ID", "Ho ten", "Phone", "So du", "Trang thai");
        printf("----------------------------------------------------------------------------------------------\n");
        for (int i = start; i < end; i++) {
            printf("| %-3d | %-15s | %-25s | %-12s | %-10.2f | %-10s |\n",
                   i + 1, accounts[i].accountId, accounts[i].fullName, accounts[i].phone, accounts[i].balance,
                   accounts[i].status == 1 ? "Hoat dong" : "Khoa");
        }
        printf("----------------------------------------------------------------------------------------------\n");

        while (1) {
            printf("||      'n' trang sau    ||   'p' trang truoc    ||   'q' thoat    ||   So trang de nhay den      ||\n");
            printf("----------------------------------------------------------------------------------------------\n");
            printf("Nhap lua chon: ");
            if (!read_line(input, sizeof(input))) { printf("Loi khi doc du lieu. Huy.\n"); return; }
            if (strlen(input) == 1) {
                char c = input[0];
                if (c == 'n' || c == 'N') {
					if (currentPage + 1 < totalPages)
						currentPage++;
					else
						printf("Ban dang o trang cuoi!\n");
						break;
					}
                else if (c == 'p' || c == 'P') {
					if (currentPage > 0)
					currentPage--;
				else
					printf("Ban dang o trang dau!\n");
					break;
				}
                else if
					(c == 'q' || c == 'Q') {
					return;
					}
            }
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
            currentPage = (int)pageLong - 1; break;
        }
    }
}

void f06_sortAccounts(struct Account accounts[], int size) {
    if (size == 0) { printf("Chua co tai khoan nao trong he thong!\n"); return; }
    char input[20];
    long choice;
    char *endptr;
    while (1) {
        printf("\n||======= SAP XEP TAI KHOAN =======||\n");
        printf("||[1] Sap xep theo SO DU (Giam dan)||\n");
        printf("||=================================||\n");
        printf("||[2] Sap xep theo TEN (A -> Z)    ||\n");
        printf("||=================================||\n");
        printf("Chon (1-2): ");
        if (!read_line(input, sizeof(input)))
			continue;
        choice = strtol(input, &endptr, 10);
        if (*endptr == '\0' && (choice == 1 || choice == 2))
			break;
        printf("Lua chon khong hop le! Vui long nhap 1 hoac 2.\n");
    }

    for (int i = 0; i < size - 1; i++) for (int j = 0; j < size - i - 1; j++) {
        int swap = 0;
        if (choice == 1) {
			if (accounts[j].balance < accounts[j + 1].balance) swap = 1;
		}
        else {
            int cmp = strcasecmp(accounts[j].fullName, accounts[j + 1].fullName);
            if (cmp > 0) swap = 1;
            else if (cmp == 0) if (strcasecmp(accounts[j].accountId, accounts[j + 1].accountId) > 0) swap = 1;
        }
        if (swap) { struct Account tmp = accounts[j]; accounts[j] = accounts[j + 1]; accounts[j + 1] = tmp; }
    }

    if (choice == 1)
		printf("Da sap xep theo SO DU (Giam dan)!\n");
	else
		printf("Da sap xep theo TEN (A -> Z)!\n");
}

void f07_transfer(struct Account accounts[], int size, struct Transaction transactions[], int *transCount) {
    if (size == 0) {
		printf("Chua co tai khoan nao trong he thong!\n");
		return;
	}

    char sendId[20], recvId[20], buf[64], confirm[10];
    int senderIdx = -1, receiverIdx = -1;

    while (1) {
        printf("Nhap ID nguoi gui: ");
        if (!read_line(sendId, sizeof(sendId)))
			continue;
        if (strlen(sendId) == 0) {
			printf("ID khong duoc de trong! Hay nhap lai.\n");
			continue;
		}
        senderIdx = find_account_by_id(accounts, size, sendId);
        if (senderIdx == -1) {
			printf("Khong tim thay tai khoan nguoi gui! Hay nhap lai.\n");
			continue;
		}
        if (accounts[senderIdx].status == 0) {
			printf("Tai khoan nguoi gui dang bi KHOA! Vui long nhap tai khoan khac.\n");
			continue;
		}
        break;
    }

    while (1) {
        printf("Nhap ID nguoi nhan: ");
        if (!read_line(recvId, sizeof(recvId)))
			continue;
        if (strlen(recvId) == 0) {
			printf("ID khong duoc de trong! Hay nhap lai.\n");
			continue;
		}
        receiverIdx = find_account_by_id(accounts, size, recvId);
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
			continue;
		}
        break;
    }

    double amount = 0.0;
    while (1) {
        if (!prompt_positive_double_original("Nhap so tien can chuyen: ", &amount)) return;
        if (amount > accounts[senderIdx].balance) {
			printf("So du khong du! Hien co: %.2f\n", accounts[senderIdx].balance);
			continue;
		}
        break;
    }

    printf("\n|--- XAC NHAN CHUYEN KHOAN ---|\n");
    printf("Nguoi gui : %s (ID: %s)\n", accounts[senderIdx].fullName, accounts[senderIdx].accountId);
    printf("Nguoi nhan: %s (ID: %s)\n", accounts[receiverIdx].fullName, accounts[receiverIdx].accountId);
    printf("So tien    : %.2f VNÐ\n", amount);
    printf("|-----------------------------|\n");

    printf("Ban co chac chan muon chuyen khoan khong? (Y/N): ");
    if (!read_line(confirm, sizeof(confirm)))
		return;
    if (confirm[0] != 'y' && confirm[0] != 'Y') {
		printf("\n>> Giao dich da bi HUY boi nguoi dung!\n");
		return;
	}

    accounts[senderIdx].balance -= amount;
    accounts[receiverIdx].balance += amount;

    if (*transCount < MAX_TX) {
        int idx = *transCount;
        snprintf(transactions[idx].transId, sizeof(transactions[idx].transId), "T%04d", idx + 1);
        strcpy(transactions[idx].senderId, accounts[senderIdx].accountId);
        strcpy(transactions[idx].receiverId, accounts[receiverIdx].accountId);
        transactions[idx].amount = amount;
        strcpy(transactions[idx].type, "TRF");
        now_str(transactions[idx].date, sizeof(transactions[idx].date));
        (*transCount)++;
    } else {
        printf(">> Warning: Bo nho lich su giao dich da day. Khong luu.\n");
    }

    printf("\n>> Chuyen tien THANH CONG!\n");
    printf("Ma giao dich: %s\n", transactions[(*transCount)-1].transId);
    printf("So du moi cua nguoi gui: %.2f\n", accounts[senderIdx].balance);
}

void f08_showTransactions(struct Transaction transactions[], int transCount, struct Account accounts[], int accSize) {
    if (accSize == 0) {
		printf("Chua co tai khoan nao trong he thong!\n");
		return;
	}
    char currentId[64];

    while (1) {
        printf("Nhap ID tai khoan de xem lich su (Enter de huy): ");
        if (!read_line(currentId, sizeof(currentId))) {
			printf("Loi khi doc du lieu.\n");
			return;
		}
        if (strlen(currentId) == 0) {
			printf("Da huy xem lich su.\n");
			return;
		}
        if (find_account_by_id(accounts, accSize, currentId) == -1) {
			printf("Khong tim thay tai khoan co ID '%s'. Vui long nhap lai!\n\n", currentId);
			continue;
		}
        break;
    }

    int matched = 0;
    printf("\n================ LICH SU GIAO DICH: %s ================\n", currentId);
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("| %-20s | %-10s | %-15s | %-15s | %-15s |\n",
           "Ngay gio", "Ma GD", "Loai GD", "Doi tac", "So tien");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < transCount; i++) {
        int isSender = (strcmp(transactions[i].senderId, currentId) == 0);
        int isReceiver = (strcmp(transactions[i].receiverId, currentId) == 0);
        if (isSender || isReceiver) {
            matched++;
            char typeDisplay[20], partnerId[20], amountString[30];
            if (isSender) {
                strcpy(typeDisplay, "GUI DI (OUT)");
                strcpy(partnerId, transactions[i].receiverId);
                snprintf(amountString, sizeof(amountString), "-%.2f", transactions[i].amount);
            } else {
                strcpy(typeDisplay, "NHAN VE (IN)");
                strcpy(partnerId, transactions[i].senderId);
                snprintf(amountString, sizeof(amountString), "+%.2f", transactions[i].amount);
            }
            printf("| %-20s | %-10s | %-15s | %-15s | %15s |\n",
                   transactions[i].date, transactions[i].transId, typeDisplay, partnerId, amountString);
        }
    }

    if (matched == 0) {
        printf("| %-96s |\n", ">> Khong co giao dich nao phu hop <<");
    }
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("Tong so giao dich: %d\n\n", matched);
}

int main() {
    struct Account accounts[MAX_ACCOUNTS];
    struct Transaction transactions[MAX_TX];
    int transCount = 0, size = 0;
    int choice;
    char input[50];

    while (1) {
        // NHAP LUA CHON MENU
        while (1) {
            displayMenu();
            printf("Moi ban nhap lua chon: ");

            if (!read_line(input, sizeof(input))) {
				printf("Loi khi nhap! Hay thu lai.\n");
				continue;
			}
            if (strlen(input) == 0) {
				printf("Khong duoc de trong! Hay nhap lai.\n");
				continue;
			}
            if (is_only_spaces(input)) {
				printf("Khong duoc chi nhap khoang trang!\n");
				continue;
			}
            if (!only_digits(input)) {
			printf("Lua chon chi duoc chua chu so!\n");
			continue;
			}

            choice = atoi(input);
            break;
        }

        switch (choice) {
            case 1:
				f01_addAccount(accounts, &size);
				break;
            case 2:
				f02_updateAccount(accounts, size);
				break;
            case 3: {
            	char choice_01;
                char input_choice[10];

                while (1) {
                    menu_f03();
                    printf("Moi ban nhap lua chon (a/b) (hoac q de thoat): ");
                    if (!read_line(input_choice, sizeof(input_choice))) {
                        printf("Loi khi doc du lieu.\n");
                        continue;
                    }
                    if (strlen(input_choice) != 1) {
                        printf("Lua chon khong hop le! Chi nhap mot ky tu (a/b/q).\n");
                        continue;
                    }

                    choice_01 = input_choice[0];
                    if (choice_01 == 'a' || choice_01 == 'A') {
                        f03_lockOrUnlock(accounts, size);
                        break;
                    } else if (choice_01 == 'b' || choice_01 == 'B') {
                        f03_deleteAccount(accounts, &size);
                        break;
                    } else if (choice_01 == 'q' || choice_01 == 'Q') {
                        break;
                    } else {
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
				f08_showTransactions(transactions, transCount, accounts, size);
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
