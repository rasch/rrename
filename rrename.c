#include <ctype.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

char *help[] = {
  "Usage",
  "",
  "    rrename [-d] [-o] [-r] [-i STRING] ...",
  "",
  "Options",
  "  -d, --dry-run",
  "      Display renamed files without actually renaming anything.",
  "",
  "  -h, --help",
  "      Show this help menu.",
  "",
  "  -i, --ignore STRING",
  "      Don't rename files that contain STRING. Use this option",
  "      multiple times (up to 32) to ignore files that contain any",
  "      of the STRINGS.",
  "",
  "  -o, --overwrite",
  "      WARNING! This option allows overwriting existing files.",
  "",
  "  -r, --recursive",
  "      Recursively rename all files in current directory.",
  "",
  "Synopsis",
  "",
  "  Rename all files in current directory using opinionated file",
  "  naming conventions. Any character that is not a-z, 0-9,",
  "  underscore (_), slash (/), period (.) or minus (-) is replaced",
  "  or removed and filenames are converted to lowercase. Non-ASCII",
  "  characters are not modified."
  "",
  "", "Examples",
  "",
  "  $ rrename",
  "  el niño.txt => el_niño.txt",
  "  I'm WAITING....txt => im_waiting.txt",
  "",
  "EOF"
};

typedef struct {
  char dryrun;
  char overwrite;
  char recursive;
  char ignore[32][128];
  unsigned short ipos;
} options;

int cmp(char *a, char *b, unsigned short n) {
  return !strncmp(a, b, n);
}

void print_doc(char *doc[]) {
  for (int i = 0; !cmp(doc[i], "EOF", 4); i++) {
    printf("%s\n", doc[i]);
  }
}

void rrename(char *path, options *opts) {
  struct dirent *ep;
  DIR *dir = opendir(path);

  if (!dir) {
    return;
  }

  while ((ep = readdir(dir))) {
    char *cur = ep->d_name;

    if (cmp(cur, ".", 2) || cmp(cur, "..", 3) || cmp(cur, ".git", 5)) {
      continue;
    }

    int ignored = 0;

    for (int i = 0; i < opts->ipos; i++) {
      if (strstr(cur, opts->ignore[i])) {
        ignored = 1;
        break;
      }
    }

    if (ignored) {
      continue;
    }

    char old_path[1024];
    char new_path[1024];

    strncpy(old_path, path, 1024);
    strncat(old_path, "/", 1);
    strncat(old_path, cur, 512);

    int i = 0;
    int j = 0;

    while (old_path[j]) {
      new_path[i] = tolower(old_path[j]);

      switch (old_path[j]) {
        case '&':
          new_path[i] = 'a';
          new_path[++i] = 'n';
          new_path[++i] = 'd';
          break;
        case '/':
        case '-':
          break;
        case '"':
        case '\'':
          i--;
          break;
        case '_':
        handle_underscore:
          switch(old_path[j + 1]) {
            case '_':
            case '.':
              i--;
              break;
            case '-':
              j++;
          }
          switch(new_path[i - 1]) {
            case '_':
            case '-':
              i--;
          }
          break;
        case '.':
          if (old_path[j + 1] == '.') {
            i--;
          }
          break;
        default:
          if (ispunct(new_path[i]) || isspace(new_path[i])) {
            new_path[i] = '_';
            goto handle_underscore;
          }
          break;
      }

      i++;
      j++;
    }

    if (ispunct(new_path[i - 1])) {
      new_path[i - 1] = '\0';
    } else {
      new_path[i] = '\0';
    }

    if (!cmp(old_path, new_path, 1024)) {
      int exists = !access(new_path, F_OK);

      if (exists) {
        printf(opts->overwrite ? "[OVERWRITING]: " : "[SKIPPING]: ");
      }

      printf("%s -> %s\n", old_path, new_path);

      if (!opts->dryrun && (!exists || opts->overwrite)) {
        if (rename(old_path, new_path) != 0) {
          printf("[ERROR]: Can't rename '%s' to '%s'.\n", old_path, new_path);
        }
      }
    }

    if (opts->recursive) {
      rrename(opts->dryrun ? old_path : new_path, opts);
    }
  }

  closedir(dir);
}

int main(int argc, char **argv) {
  options opts;

  opts.recursive = 0;
  opts.dryrun = 0;
  opts.overwrite = 0;
  opts.ipos = 0;

  for (int i = 1; i < argc; i++) {
    if (cmp(argv[i], "-r", 3) || cmp(argv[i], "--recursive", 12)) {
      opts.recursive = 1;
    } else if (cmp(argv[i], "-d", 3) || cmp(argv[i], "--dry-run", 10)) {
      opts.dryrun = 1;
    } else if (cmp(argv[i], "-h", 3) || cmp(argv[i], "--help", 7)) {
      print_doc(help);
      return 0;
    } else if (cmp(argv[i], "-o", 3) || cmp(argv[i], "--overwrite", 12)) {
      opts.overwrite = 1;
    } else if (cmp(argv[i], "-i", 3) || cmp(argv[i], "--ignore", 9)) {
      if (opts.ipos <= 32) {
        strncpy(opts.ignore[opts.ipos++], argv[++i], 128);
      }
    } else if (argv[i][0] == '-') {
      int n = strlen(argv[i]);

      for (int j = 1; j < n; j++) {
        switch (argv[i][j]) {
          case 'd':
            opts.dryrun = 1;
            break;
          case 'o':
            opts.overwrite = 1;
            break;
          case 'r':
            opts.recursive = 1;
            break;
          case 'i':
            if (j == n - 1 && opts.ipos <= 32) {
              strncpy(opts.ignore[opts.ipos++], argv[++i], 128);
            }
            break;
          default:
            print_doc(help);
            return 1;
        }
      }
    } else {
      print_doc(help);
      return 1;
    }
  }

  rrename(".", &opts);

  return 0;
}
