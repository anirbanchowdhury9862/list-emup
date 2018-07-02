#include<stdlib.h>
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fts.h>
#include<string.h>
#include<errno.h>
#include <unistd.h>
#include <time.h>

int compare(const FTSENT **, const FTSENT **);
int sizecmp(const FTSENT *, const FTSENT *);
int namecmp(const FTSENT *, const FTSENT *);
int acccmp(const FTSENT *, const FTSENT *);
int statcmp(const FTSENT *, const FTSENT *);
int modcmp(const FTSENT *, const FTSENT *);
int (*sortfnc) (const FTSENT *, const FTSENT *);

int main(int argc, char *const argv[])
{
	int choice, i = 0;
	int f_namesort;
	int f_sizesort;
	int f_accesstimesort;
	int f_statuschangesort;
	int f_modificationsort;
	char ch[128];

	FTS *file_system = NULL;
	FTSENT *parent = NULL;

	if (argc < 1) {
		printf("Usage: %s <path-spec>\n", argv[0]);
		exit(255);
	}

	do {
		i = 0;
		printf
		    ("enter your choice of sorting\n1.namesort\n2.sizesort\n3.accesstimesort\n4.statuschangetimesort\n5.modificationtimesort\n");
		scanf("%d", &choice);
		switch (choice) {

		case 1:
			sortfnc = namecmp;
			f_namesort = 1;
			f_sizesort = 0;
			f_accesstimesort = 0;
			f_statuschangesort = 0;
			f_modificationsort = 0;
			break;
		case 2:
			sortfnc = sizecmp;
			f_namesort = 0;
			f_sizesort = 1;
			f_accesstimesort = 0;
			f_statuschangesort = 0;
			f_modificationsort = 0;

			break;
		case 3:
			sortfnc = acccmp;
			f_namesort = 0;
			f_sizesort = 0;
			f_accesstimesort = 1;
			f_statuschangesort = 0;
			f_modificationsort = 0;

			break;
		case 4:
			sortfnc = statcmp;
			f_namesort = 0;
			f_sizesort = 0;
			f_accesstimesort = 0;
			f_statuschangesort = 1;
			f_modificationsort = 0;

			break;
		case 5:
			sortfnc = modcmp;
			f_namesort = 0;
			f_sizesort = 0;
			f_accesstimesort = 0;
			f_statuschangesort = 0;
			f_modificationsort = 1;

			break;
		default:
			printf("\nwe assume you want to exit");
			exit(1);
			break;

		}

		if (argc != 2) {
			strcpy(argv[0], getcwd(ch, 128));

			file_system =
			    fts_open(argv + 0, FTS_PHYSICAL | FTS_XDEV,
				     &compare);
		}

		else {
			file_system =
			    fts_open(argv + 1, FTS_PHYSICAL | FTS_XDEV,
				     &compare);
		}

		if (NULL != file_system) {
			while ((parent = fts_read(file_system)) != NULL) {
				if (i == 0) {
					strcpy(ch, parent->fts_name);
					i++;
				}
				if (strcmp(ch, parent->fts_name) != 0) {
					fts_set(file_system, parent, FTS_SKIP);
					if (f_namesort)
						printf("\n%llu\t\t%s",
						       parent->fts_statp->
						       st_size,
						       parent->fts_name);

					if (f_statuschangesort)
						printf("\n%s\t\t%s",
						       ctime(&parent->
							     fts_statp->
							     st_ctime),
						       parent->fts_name);

					if (f_modificationsort)
						printf("\n%s\t\t%s",
						       ctime(&parent->
							     fts_statp->
							     st_mtime),
						       parent->fts_name);

					if (f_accesstimesort)
						printf("\n%s\t\t%s",
						       ctime(&parent->
							     fts_statp->
							     st_atime),
						       parent->fts_name);

					if (f_sizesort)
						printf("\n%llu\t\t%s",
						       parent->fts_statp->
						       st_size,
						       parent->fts_name);

				}

			}

			fts_close(file_system);
		}
	} while (1);
	return 0;
}

int compare(const FTSENT ** one, const FTSENT ** two)
{
	return (*sortfnc) (*one, *two);
}

int sizecmp(const FTSENT * a, const FTSENT * b)
{

	if (a->fts_statp->st_size > b->fts_statp->st_size)
		return 1;
	else if (a->fts_statp->st_size < b->fts_statp->st_size)
		return -1;
	else
		return namecmp(a, b);
}

int namecmp(const FTSENT * a, const FTSENT * b)
{
	return (strcoll(a->fts_name, b->fts_name));
}

int acccmp(const FTSENT * a, const FTSENT * b)
{
	if (b->fts_statp->st_atime > a->fts_statp->st_atime)
		return (1);
	else if (b->fts_statp->st_atime < a->fts_statp->st_atime)
		return (-1);

	else
		return (namecmp(a, b));
}

int statcmp(const FTSENT * a, const FTSENT * b)
{
	if (b->fts_statp->st_ctime > a->fts_statp->st_ctime)
		return (1);
	else if (b->fts_statp->st_ctime < a->fts_statp->st_ctime)
		return (-1);

	else
		return (namecmp(a, b));

}

int modcmp(const FTSENT * a, const FTSENT * b)
{
	if (b->fts_statp->st_mtime > a->fts_statp->st_mtime)
		return (1);
	else if (b->fts_statp->st_mtime < a->fts_statp->st_mtime)
		return (-1);

	else
		return (namecmp(a, b));
}
