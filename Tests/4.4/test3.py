
import graph as gw
#basic test

graph1 = gw.create()
gw.addVertex(graph1, 'v1')
gw.addVertex(graph1, 'v2')
gw.addEdge(graph1, 'v1','v2')
gw.addVertex(graph1, 'v3')
gw.disp(graph1)

graph2 = gw.create()
gw.addVertex(graph2, 'v2')
gw.addVertex(graph2, 'v3')
gw.addEdge(graph2, 'v2','v3')
gw.disp(graph2)

graph3 = gw.create()
graph3 = gw.graphUnion(graph1, graph2, graph3)
gw.disp(graph3)

gw.destroy(graph1)
gw.destroy(graph2)
gw.destroy(graph3)
