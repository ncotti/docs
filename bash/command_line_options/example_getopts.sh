#!/bin/bash

###############################################################################
# Example retrieval of command line options with getopts.
#
# The /usr/bin/getopt is a bash command. The getopts is a shell built-in. The
# latter will be used (since it's a built-in, see docs with "help getopts").
#
# The command accepts two parameters:
# 	getopts optstring name [arg ...]
#
# Called from a loop, it will use this variables:
# 	$name: the left most option chosen.
# 	$OPTIND: the current option index from the command line.
# 	$OPTARG: the option associated value (if any).
#
# Getopts will accept as valid options:
#	-a 				(option "a")
#	-ab 			(option "a" and option "b", separated)
#	-a "xxx"		(option "a" with value "xxx")
#	-axxx			(option "a" with value "xxx")
#	--				(End of options, it will return not zero)
#
# The opstring determines which short options are valid. It accepts the following format:
# 	a				Option "-a"
#	a:				Option "-a value" with required argument.
#   : (first char)	Silent reporting (see help getopts)
#
# By default, long arguments aren't parsed. They are read as:
# --long_argument=value  ==> option "-", with value "long_argument=value"
# Using regex, the option and value are separated from the equal "=" sign, and
# parsed neverttheless.
###############################################################################

is_required_arg() {
	# Checking if the argument doesn't not start with a colon "-"
	local regex="^[^-]"
	echo "$1" | grep -P "$regex" &>/dev/null
	return $?
}


# End the optstring with "-:" to accept long options.
optstring=":abc:d:-:"

while getopts $optstring name; do
	case "$name" in
		# Add here short options with no required args
		a) echo "Option -a";;

		b) echo "Option -b";;

		# Add here short options with required args.
		# It's important to call "is_required_arg" to prevent -c -a being
		# parsed as option "-c" with value "-a", instead of a missed argument.
		c) {
			if is_required_arg "$OPTARG"; then
				echo "Option -c with value: $OPTARG"
			else
				echo "Missing value for option -c"
				exit 1
			fi
		};;

		d) {
			if is_required_arg "$OPTARG"; then
				echo "Option -d with value: $OPTARG"
			else
				echo "Missing value for option -d"
				exit 1
			fi
		};;

		-) {
			case "$OPTARG" in
				# Add here long options with no parameters.
				verbose) echo "Option --verbose";;
				clear) echo "Option --clear";;

				# Add here long options with parameters (again), to check if missing
				# required args.
				number) {
					echo "Missing value for option --number"
					exit 1
				};;
				string) {
					echo "Missing value for option --string"
					exit 1
				};;

				*) {
					# Check if it has the structure "option=value" and separate it
					regex="^[\w-]+=[\w-]+$"
					if echo "$OPTARG" | grep -P "$regex" &>/dev/null; then
						regex="(?<==)[\w-]+$"
						value=$(echo "$OPTARG" | grep -P -o "$regex")

						regex="^[\w-]+(?==)"
						option=$(echo "$OPTARG" | grep -P -o "$regex")

						case "$option" in
							# Add here long options with parameters
							number) echo "Option --number with value: $value";;
							string) echo "Option --string with value: $value";;
							*) {
								echo "Unknown option: --$option"
								exit 1
							};;
						esac
					else
						echo "Unknown option: --$OPTARG"
						exit 1
					fi
				};;
			esac
		};;

		:) echo "Missing value for option -$OPTARG";;
		\?) {
			echo "Unknown option: -$OPTARG"
			exit 1
		};;
	esac
done

shift $(( OPTIND - 1 ))

for arg in "$@"; do
	echo "Argument: $arg"
done

exit 0
