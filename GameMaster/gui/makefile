sourcefiles = $(wildcard *.java)

classes = $(sourcefiles:.java=.class)


%.class : %.java
	javac $<

jars: $(classes)
	jar cfm AgentTester.jar AgentTester-manifest.txt *.class
	jar cfm GameMaster.jar GameMaster-manifest.txt *.class


.PHONY=all
all: jars


clean:
	rm *.class AgentTester.jar GameMaster.jar
