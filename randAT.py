import webbrowser           # opens website URLs in default browser
from Tkinter import *       # GUI (Python 2.7 version)
import random

# Global strings
title = 'Press the button for a random Adventure Time episode!\n\n'
ranButton = 'Mathematical!'
spacer = '\n\n'
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

# def gui():
#   root = Tk()

#   # Bring the window to front
#   root.lift()
#   # Set minimium window size (currently just constrains to minimul grid size)
#   root.minsize(200, 50)
#   # Don't allow window resizing
#   root.resizable(0, 0)

#   root.title("Random Adventure Time")

#   mainframe = ttk.Frame(root, padding="3 3 12 12")
#   mainframe.grid(column=0, row=0, sticky=(N, W, E, S))
#   mainframe.columnconfigure(0, weight=1)
#   mainframe.rowconfigure(0, weight=1)

#   # Title label
#   ttk.Label(mainframe, text=title).grid(column=0, row=0, sticky=(W, E))

#   # Button to start open URl event/function
#   ttk.Button(mainframe, text=ranButton, command=openURL).grid(column=0, row=1, sticky=(W, E))


#   # Adds padding around each element
#   for child in mainframe.winfo_children(): child.grid_configure(padx=5, pady=5)


#   # root.bind('<Return>', openURL)

#   root.mainloop()


class Application(Frame):
    def openURL(self):
        # Generate season and episode
        season = random.choice(list(episode_dic.keys()))
        episode = random.choice(list(episode_dic[season]))

        curr_url = u'{0}{1}{2}{3}'.format(url, season, '-episode-', episode)

        webbrowser.open(curr_url, new=2, autoraise=True)


    def createWidgets(self):
        # Make title lable
        self.lb = Label(self)
        self.lb["text"] = title

        self.lb.pack({"side": "top"})

        # Make random episode button
        self.btn = Button(self)
        self.btn["text"] = ranButton
        # self.btn["fg"]   = "red"
        self.btn["command"] = self.openURL

        self.btn.pack({"side": "top"})

        self.sp = Label(self)
        self.sp["text"] = spacer

        self.sp.pack({"side": "top"})

    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()


def main():
    root = Tk()
    # root.minsize(100, 200)
    root.title("Random Adventure Time")
    app = Application(master=root)
    app.mainloop()
    root.destroy()


if __name__ == '__main__': main()