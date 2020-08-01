
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <libgen.h>
#include <math.h>

typedef enum {
	NONE      = 0b0000000,
	KILL      = 0b0000001,
	PREDATOR  = 0b0000010,
	CHIMES    = 0b0000100,
	RADIO     = 0b0001000,
	MODULATOR = 0b0010000
} voice;

int print_help(char * bin){
	
	char * base = basename(bin);
	
	printf("Usage:\n");
	printf("\t%s [-h] [-kpcr] -i INFILE -o OUTFILE\n", base);
	printf("\n");
	printf("Required:\n");
	printf("\t-h  help      -- prints out this message\n");
	printf("\t-i  input     -- path to the input file\n");
	printf("\t-?  voice     -- at least one voice flag\n");
	printf("\n");
	printf("Voices:\n");
	printf("\t-k  kill      -- flattens all but every 11th byte\n");
	printf("\t-p  predator  -- sounds like the predator, spikes levels\n");
	printf("\t-c  chimes    -- reduce sound to chimes\n");
	printf("\t-r  radio     -- old tinny radio sound\n");
	printf("\t-m  modulator -- sine wav ring modulator\n");
	printf("\n");
	
	return 1;
}

int main(int argc, char **argv){
	
	voice v = NONE;
	
	char *incname = NULL;
	char *outname = NULL;
	int index;
	int c;
	
	opterr = 0;

	while ((c = getopt (argc, argv, "hkpcrmi:o:")) != -1)
		switch (c){
			case 'k':
				v |= KILL;
				break;
			case 'p':
				v |= PREDATOR;
				break;
			case 'c':
				v |= CHIMES;
				break;
			case 'r':
				v |= RADIO;
				break;
			case 'm':
				v |= MODULATOR;
				break;
			case 'h':
				return print_help(argv[0]);
			case 'i':
				incname = optarg;
				break;
			case 'o':
				outname = optarg;
				break;
			case '?':
				if (optopt == 'i' || optopt == 'o' || optopt == 'k')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				return 0;
			default:
				return 1;
		}
	
	for (index = optind; index < argc; index++)
		printf ("Non-option argument %s\n", argv[index]);
    
    if (v == NONE || incname == NULL || outname == NULL){
		printf("E: Missing a required parameter (voice/input/output)\n");
		return print_help(argv[0]);
	}
	
	int rc = 0;
	
	FILE *inc = NULL;
	FILE *out = NULL;
	
	if( access(incname, R_OK) == -1) {
		printf("E: no readable file '%s' was found\n", incname);
		rc = 1;
		goto safe_fail;
	}
	
	if( access(outname, F_OK) != -1) {
		rc = remove(outname);
		if( rc != 0 || access(outname, F_OK) != -1){
			printf("E: file '%s' already exists, unable to delete\n", outname);
			rc = 2;
			goto safe_fail;
		}
	}
	
	inc = fopen(incname, "rb");
	
	int seekrc = fseek(inc, 0L, SEEK_END);
	if(seekrc != 0){
		printf("E: seek on '%s' failed\n", incname);
		rc = seekrc;
		goto safe_fail;
	}
	
	rewind(inc);
	
	seekrc = fseek(inc, 0L, SEEK_SET);
	if(seekrc != 0){
		printf("E: seek on '%s' failed\n", incname);
		rc = seekrc;
		goto safe_fail;
	}
	
	out = fopen(outname, "wb");
	
	int count = 0;
	int getrc = 0;
	for(;;){
		
		getrc = fgetc(inc);
		if (getrc == EOF)
			break;
		
		if (count < 44) {
			
			fputc(getrc, out);
			count++;
			continue;
		}
		
		unsigned char outbyte = 128;
		
		if (v & RADIO){
			if (count % 17 == 0)
				outbyte = getrc;
		}
		
		if (v & KILL){
			if (count % 11 == 0){
				outbyte = getrc;
			}
		}
		
		if (v & PREDATOR){
			//~ if (count % 11 != 0)
				//~ outbyte = 128;
			//~ if (count % 7 != 0)
				//~ outbyte = 128;
			//~ if (count % 5 != 0)
				//~ outbyte = 128;
			if (count % 3 == 0){
				//~ outbyte = getrc - count;
				outbyte = getrc * 10;
			}
		}
		
		if (v & CHIMES){
			//~ if (count % 11 != 0)
				//~ outbyte = 128;
			//~ if (count % 7 != 0)
				//~ outbyte = 128;
			//~ if (count % 5 != 0)
				//~ outbyte = 128;
			if (count % 3 == 0 && getrc != 0){
				outbyte = count % getrc;
			}
		}
		
		if (v & MODULATOR){
			unsigned int tmpbase = (getrc * (sin( (float)(count % 8000) / 8000 * M_PI * 2) + 1.0));
			if(tmpbase > 255)
				tmpbase = 255;
			//~ printf("%03u %03u %f\n", base[i], tmpbase, (sin( (float)i / 128.0 * M_PI * 2) + 1.0));
			//~ outbyte = getrc * sin(2 * M_PI * 500 * (count % 8000) / 7999);
			outbyte = tmpbase;
		}

		write:
		fputc(outbyte, out);
		count++;
		
	}
	
	safe_fail:
	
	if (inc != NULL) fclose(inc);
	if (out != NULL) fclose(out);
	
	return rc;
}
