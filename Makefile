NAME        = codexion
CC          = cc
CFLAGS      = -Wall -Wextra -Werror -pthread -I. -g

# Archivos en la raíz
SRC_ROOT    = codexion.c

# Archivos en la carpeta init/
SRC_INIT    = init/init_coders.c \
              init/init_data.c \
              init/init_dongle.c \
              init/parser.c

# Archivos en la carpeta src/
SRC_FILES   = src/dongle_manager.c \
              src/heap.c \
              src/monitor.c \
              src/routine.c \
              src/utils.c

SRCS        = $(SRC_ROOT) $(SRC_INIT) $(SRC_FILES)
OBJS        = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re