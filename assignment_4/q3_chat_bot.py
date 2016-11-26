import sys
from random import randint

def sanity_check(argv):
	if len(sys.argv) != 2:
		print "Please provide only the file name"
		sys.exit(0)

def process_file(text_file):
	word_pair_list = {};
	line = text_file.read()
	line = line.replace('.', ' .')
	line = line.replace('!', ' !')
	line = line.replace('?', ' ?')
	line = line.replace('-', ' ')
	line = line.replace('\n', ' ')
	line = ''.join([i for i in line if (i.isalpha() or i.strip(' .!?') == '')])
	
	words = line.split(' ')
	words = filter(lambda word:word != '', words)
	for i in xrange(len(words)-1):
		if words[i].strip('.!?') == '':
			continue
		
		word_pair = words[i] + '-' + words[i+1]
		word_pair = word_pair.lower()
		
		if word_pair in word_pair_list:
			word_pair_list[word_pair] += 1
		else:
			word_pair_list[word_pair] = 1

	word_pair_list = sorted(word_pair_list.items(), key=lambda x:x[1], reverse=True)
	return word_pair_list

def look_up_word(current_word, word_pair_list):
	for key in word_pair_list:
		if current_word == key[0].split('-')[0]:
			return True
	return False

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

def random_choose_word(word_pair_list):
	index = randint(0,len(word_pair_list))
	return word_pair_list[index][0].split('-')[0]

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

def format_msg(reply):
	formatted_reply = list(reply)
	formatted_reply[0] = reply[0].upper()
	formatted_reply[-3] = ''
	return "".join(formatted_reply)

def start_chat(word_pair_list):
	while True:
		input = raw_input("Send: ")
		last_word = input.split(' ')[-1].strip(' ,.!?\'\{\}[]#$%^&*\"\"')
		reply = generate_reply(last_word, word_pair_list)
		reply = format_msg(reply)
		print "Received: " + reply

def print_words(word_pair_list):
	for (k,v) in word_pair_list:
		print k+':'+str(v)

def main():
	sanity_check(sys.argv);
	try:
		text_file = open(sys.argv[1]);
	except Exception as e:
		print "File provided does not exist"
		sys.exit(0)
	
	word_pair_list = process_file(text_file)
	start_chat(word_pair_list)
if __name__ == '__main__':
	main()