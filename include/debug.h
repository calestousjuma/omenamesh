
// #include <stdio.h>
#ifndef DEBUG
#define DEBUG

#define OMENA_MESH_TODO(todo, ...) ;

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define BLUE "\033[1;34m"
#define RESET "\033[0m"
#define UNDERLINE "\033[4m"

#define LOG_LEVEL_INFO 1
#define LOG_LEVEL_WARN 2
#define LOG_LEVEL_ERROR 3
#define LOG_LEVEL_DEBUG 4

#ifndef LOG_LEVEL
/* Default log level is DEBUG */
#define LOG_LEVEL LOG_LEVEL_DEBUG
#endif /*! LOG_LEVEL*/

#define OMENA_MESH_LOG(level, fmt, ...)                                       \
	do {                                                                  \
		const char *level_str = "";                                   \
		const char *color = "";                                       \
		if ((level) >= LOG_LEVEL) {                                   \
			switch (level) {                                      \
				case LOG_LEVEL_INFO:                          \
					level_str = "INFO";                   \
					color = GREEN;                        \
					break;                                \
				case LOG_LEVEL_WARN:                          \
					level_str = "WARN";                   \
					color = YELLOW;                       \
					break;                                \
				case LOG_LEVEL_ERROR:                         \
					level_str = "ERROR";                  \
					color = RED;                          \
					break;                                \
				case LOG_LEVEL_DEBUG:                         \
					level_str = "DEBUG";                  \
					color = BLUE;                         \
					break;                                \
				default:                                      \
					level_str = "UNKNOWN";                \
					color = RESET;                        \
					break;                                \
			}                                                     \
			fprintf(stderr, "%s%s: %s:%d: " fmt "%s\n", color,    \
				level_str, __FILE__, __LINE__, ##__VA_ARGS__, \
				RESET);                                       \
			fflush(stderr);                                       \
		}                                                             \
	} while (0)

#define INFO_(fmt, ...) OMENA_MESH_LOG(LOG_LEVEL_INFO, fmt, ##__VA_ARGS__)

#define UNREACHABLE()                                                   \
	do {                                                            \
		\ fprintf(stderr,                                       \
			  "ERROR: Unreachable code reached at %s:%d\n", \
			  __FILE__, __LINE__);                          \
		exit(0);                                                \
	} while (0)

#define DEBUG_PRINT(fmt, ...)                                             \
	do {                                                              \
		fprintf(stderr, "DEBUG: %s:%d: " fmt, __FILE__, __LINE__, \
			##__VA_ARGS__);                                   \
	} while (0)

#ifdef DEBUG
#define VERBOSE_DEBUG(fmt, ...)                                             \
	do {                                                                \
		fprintf(stderr, "VERBOSE: %s:%d: " fmt, __FILE__, __LINE__, \
			##__VA_ARGS__);                                     \
	} while (0)
#else
#define VERBOSE_DEBUG(fmt, ...) \
	do {                    \
	} while (0)
#endif

#define OMENA_MESH_MEMORY_DUMP(ptr, size)                                    \
	do {                                                                 \
		u8__CJLF *byte_ptr = (u8__CJLF *)(ptr);                      \
		fprintf(stderr, "MEMORY DUMP (%zu bytes):\n", size);         \
		for (i64__CJLF i = 0;                                        \
		     i < (i64__CJLF)size /*make the compiler happy*/; i++) { \
			fprintf(stderr, "%08x ", byte_ptr[i]);               \
			if ((i + 1) % 16 == 0)                               \
				fprintf(stderr, "\n");                       \
		}                                                            \
		fprintf(stderr, "\n");                                       \
	} while (0)

#define LOG_FUNC_ENTRY()                                              \
	do {                                                          \
		fprintf(stderr, "Entering function: %s\n", __func__); \
	} while (0)

#define LOG_FUNC_EXIT()                                              \
	do {                                                         \
		fprintf(stderr, "Exiting function: %s\n", __func__); \
	} while (0)

#endif /*! DEBUG*/