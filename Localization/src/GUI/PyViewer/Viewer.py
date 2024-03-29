# Viewer.py
from graphics import *
from time import sleep

filename = 'ParticleLists.txt'
# filename = 'Perspectives.txt'

def main(last):

    win = GraphWin('Points', 570, 570)

    while True:

        f = open(filename)

        particles = []

        minx = 200
        maxx = 0
        miny = 200
        maxy = 0

        maxw = 0
        minw = 100

        for line in f:
            oldp = line.split()
            p = []
            for i in oldp:
                p += [float(i)]
            p.pop(2)
            p.pop(4)
            particles += [p]

            if p[0] < minx:
                minx = p[0]
            elif p[0] > maxx:
                maxx = p[0]
            if p[1] < miny:
                miny = p[1]
            elif p[1] > maxy: 
                maxy = p[1]
            if p[4] > maxw:
                maxw = p[4]
            elif p[4] < minw and p[4] > 11:
                minw = p[4]
            last = line

        f.close()

        try:
            w = maxx - minx
            h = maxy - miny

            mulw = 255 / maxw;

            l = Line(Point(20, 20), Point(20,530))
            l.setArrow("both")
            l.draw(win)

            l = Line(Point(50, 560), Point(520,560))
            l.setArrow("both")
            l.draw(win)

            avgx = (maxx + minx) / 2.0
            avgy = (maxy + miny) / 2.0

            l = Line(Point(15, 10+(avgy - miny) * 550/h), Point(25, 10+(avgy - miny) * 550/h))
            l.draw(win)

            l = Line(Point(10+(avgx - minx) * 550/w, 555), Point(10+(avgx - minx) * 550/w, 565))
            l.draw(win)

            t = Text(Point(20, 10), str(maxy))
            t.draw(win)
            t = Text(Point(550, 560), str(maxx))
            t.draw(win)
            t = Text(Point(20, 540), str(miny))
            t.draw(win)
            t = Text(Point(20, 560), str(minx))
            t.draw(win)
            t = Text(Point(45, 10+(avgy - miny) * 550/h), str(avgy))
            t.draw(win)
            t = Text(Point(10+(avgx - minx) * 550/w, 540), str(avgx))
            t.draw(win)
            x = 10+(particles[0][0] - minx) * 550/w 
            y = 570 - (10+(particles[0][1] - miny) * 550/h)
            c = Circle(Point(x, y), 8)
            c.setFill("green")
            c.draw(win)
            dx = 10 + ( particles[0][0] + particles[0][2] * 0.2 - minx) * 550 / w
            dy = 570 - (10 + ( particles[0][1] + particles[0][3] * 0.2 - miny) * 550 / h)
            l = Line(Point(x, y), Point(dx, dy))
            l.setArrow("last")
            l.draw(win)
            
            particles.pop(0)

            x = 10+(particles[0][0] - minx) * 550/w 
            y = 570 - (10+(particles[0][1] - miny) * 550/h)
            c = Circle(Point(x, y), 8)
            c.setFill("yellow")
            c.draw(win)
            dx = 10 + ( particles[0][0] + particles[0][2] * 0.2 - minx) * 550 / w
            dy = 570 - (10 + ( particles[0][1] + particles[0][3] * 0.2 - miny) * 550 / h)
            l = Line(Point(x, y), Point(dx, dy))
            l.setArrow("last")
            l.draw(win)
            
            particles.pop(0)

            for point in particles:
                x = 10+(point[0] - minx) * 550/w 
                y = 570 - (10+(point[1] - miny) * 550/h)
                dx = 10 + ( point[0] + point[2] * 0.2 - minx) * 550 / w
                dy = 570 - (10 + ( point[1] + point[3] * 0.2 - miny) * 550 / h)

                wt = point[4]

                c = Circle(Point(x, y), 4)
                c.setFill(color_rgb(max(0, min(255, (wt - minw) * 255/(maxw - minw))), 0, max(0, min(255, 255 - (wt - minw) * 255/(maxw - minw)))))
                c.draw(win)
                l = Line(Point(x, y), Point(dx, dy))
                l.setArrow("last")
                l.draw(win)
                # print "Point", x - minx, y - miny

        except:
            t = Text(Point(285, 285), "Error. Please wait while I reconfigure.")
            t.draw(win)
        
        new = False

        while not new:
            f = open(filename)
            for line in f:
                newlast = line
            if last == newlast:
                sleep(1)
            else:
                last = newlast
                new = True
            f.close()
            sleep(0.2)

        win.delete("all")


if __name__ == "__main__": main("")