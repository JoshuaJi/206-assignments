import sys
from random import randint

# sanity chekc the number of arguments
def sanity_check(argv):
	if len(sys.argv) < 2:
		print "Please provide at least one file name"
		sys.exit(0)

# take a text file and extracts the word-pair as well as their frequency
def process_file(text_file):
	word_pair_list = {};
	line = text_file.read()
	line = line.replace('.', ' .')
	line = line.replace('!', ' !')
	line = line.replace('?', ' ?')
	line = line.replace('-', ' ')
	line = line.replace('\n', ' ')
	line = ''.join([i for i in line if (i.isalpha() or i.strip(' .!?') == '')])
	
	# split the artical into words and ignore the empty ones
	words = line.split(' ')
	words = filter(lambda word:word != '', words)
	for i in xrange(len(words)-1):
		# if the first word is ending the sentence, skip the word
		if words[i].strip('.!?') == '':
			continue
		
		# construct word pair and add it to the dictionary
		word_pair = words[i] + '-' + words[i+1]
		word_pair = word_pair.lower()
		
		if word_pair in word_pair_list:
			word_pair_list[word_pair] += 1
		else:
			word_pair_list[word_pair] = 1

	# sort word-pair dictionary based on their frequency
	word_pair_list = sorted(word_pair_list.items(), key=lambda x:x[1], reverse=True)
	return word_pair_list

# check if a word is in the word-pair dictionary
def look_up_word(current_word, word_pair_list):
	for key in word_pair_list:
		if current_word == key[0].split('-')[0]:
			return True
	return False

# given a word, find the corresponding pair if it exists in the dictionary,
# if it doesn't exist, return an empty string
def generate_next_word(current_word, word_pair_list):
	if current_word+'-.' in word_pair_list:
		return '.'
	if current_word+'-!' in word_pair_list:
		return '!'
	if current_word+'-?' in word_pair_list:
		return '?'
	for key in word_pair_list:
		if current_word == key[0].split('-')[0]:
			return key[0].split('-')[1]
	return ''

# randomly choose a word from word-pair list
def random_choose_word(word_pair_list):
	index = randint(0,len(word_pair_list))
	return word_pair_list[index][0].split('-')[0]

# given a starting word, generate a whole sentence based on the word-pair list
def generate_reply(last_word, word_pair_list):
	reply = ""
	i = 0
	if look_up_word(last_word, word_pair_list):
		current_word = last_word
	else:
		current_word = random_choose_word(word_pair_list)
	reply += current_word
	reply += ' '
	while i < 20:
		current_word = generate_next_word(current_word, word_pair_list)
		reply += current_word
		reply += ' '
		if current_word == '' or current_word == '.' or current_word == '!' or current_word == '?':
			return reply
		i += 1
	return reply

# capitalize the reply
def format_msg(reply):
	formatted_reply = list(reply)
	formatted_reply[0] = reply[0].upper()
	formatted_reply[-3] = ''
	return "".join(formatted_reply)

# start the chating loop
def start_chat(word_pair_list):
	while True:
		input = raw_input("Send: ")
		last_word = input.split(' ')[-1].strip(' ,.!?\'\{\}[]#$%^&*\"\"')
		reply = generate_reply(last_word, word_pair_list)
		reply = format_msg(reply)
		print "Received: " + reply

def current_count(item, word_pair_list):
	for pair in word_pair_list:
		if pair[0]==item[0]:
			return pair[1]
	return -1

def merge_lists(list1, list2):
	result_list = {}
	for item in list1:
		if(item[0] in result_list):
			retult_list[item[0]] = item[1]+result_list[item[0]]
		else:
			result_list[item[0]] = item[1]

	for item in list2:
		if(item[0] in result_list):
                        result_list[item[0]] = item[1]+result_list[item[0]]
                else:
                        result_list[item[0]] = item[1]

	result_list = sorted(result_list.items(), key=lambda x:x[1], reverse=True)
	return result_list

def main():
	sanity_check(sys.argv);
        main_word_pair_list = []
	for file in sys.argv[1:]:
		try:
			print "===="+file+"===="
			text_file = open(file);
			print "file opened"
			word_pair_list = process_file(text_file)
                	print "file process done"
			main_word_pair_list = merge_lists(word_pair_list, main_word_pair_list)
			print "word pair merged"
			text_file.close()
		except Exception as e:
			print e
			print "File %s provided does not exist"%(file)
	
	start_chat(main_word_pair_list)
if __name__ == '__main__':
	main()
