import webbrowser			# opens website URLs in default browser
from tkinter import *		# GUI
from tkinter import ttk
import random

# Global strings
title = 'Press the button for a random Adventure Time episode!'
ranButton = 'Mathematical!'
# Watch Cartoons default site
url = 'https://www.watchcartoononline.io/adventure-time-season-'
selection = 'Episode selected: '
# Season and episode restrictions
episode_dic = {
					'1':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26'),
					'2':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26'),
					'3':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19-20','21','22','23','24','25','26'),
					'4':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26'),
					'5':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26',
						'27','28','29','30','31','32','33','34','35','36','37','38','39','40','41','42','43','44','45','46','47','48','49','50-51','52'),
					'6':('1','2','3','4','5','6','7','8','9','10','11','12','13','14','15','16','17','18','19','20','21','22','23','24','25','26',
						'27','28','29','30','31','32','33','34','35','36','37','38','39','40','41','42','43'),
					'7':('1','2','3','4','5','6','7','8','9','10','11','12','13','14-15','16','17','18','19','20','21','22','23','24','25','26',
						'27','28','29','30','31','32','33','34','35','36','37','38','39')
				}

def gui():
	root = Tk()

	# Bring the window to front
	root.lift()
	# Set minimium window size (currently just constrains to minimul grid size)
	root.minsize(200, 50)
	# Don't allow window resizing
	root.resizable(0, 0)

	root.title("Random Adventure Time")

	mainframe = ttk.Frame(root, padding="3 3 12 12")
	mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
	mainframe.columnconfigure(0, weight=1)
	mainframe.rowconfigure(0, weight=1)

	# Title label
	ttk.Label(mainframe, text=title).grid(column=0, row=0, sticky=(W, E))

	# Button to start open URl event/function
	ttk.Button(mainframe, text=ranButton, command=openURL).grid(column=0, row=1, sticky=(W, E))


	# Adds padding around each element
	for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)


	# root.bind('<Return>', openURL)

	root.mainloop()


def openURL():
	# Generate season and episode
	season = random.choice(list(episode_dic.keys()))
	episode = random.choice(list(episode_dic[season]))

	curr_url = u'{0}{1}{2}{3}'.format(url, season, '-episode-', episode)

	# curr_url = 'http://kisscartoon.me/Cartoon/Adventure-Time-with-Finn-Jake-Season-05/Episode-043'

	# print(season,episode)
	# print(curr_url)

	webbrowser.open(curr_url, new=2, autoraise=True)

def main():
	gui()



if __name__ == '__main__': main()