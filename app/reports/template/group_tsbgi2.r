library(ggplot2)
library(dplyr)
library(crayon)

printf <- function(...) cat(sprintf(...))
prnf <- function(...) cat(sprintf("%f ", ...))
prnd <- function(...) cat(sprintf("%d ", ...))
prln <- function(...) cat(sprintf("\n"))

blue_printf   <- function(...) cat(blue(...))
white_printf <- function(...) cat(white(sprintf(...)))
silver_printf <- function(...) cat(silver(sprintf(...)))

# RowNum,ProblemNum,Kernel,IndRunNum,wADF,ADF,Types,Constraints,acgpwhat,MaxTreeDepth,GenNum,SubPopNum,MeanStdFitnessOfGen,StdFitnessBestOfGenInd,StdFitnessWorstOfGenInd,MeanTreeSizeOfGen,MeanTreeDepthOfGen,TreeSizeBestOfGenInd,TreeDepthBestOfGenInd,TreeSizeWorstOfGenInd,TreeDepthWorstOfGenInd,MeanStdFitnessOfRun,StdFitnessBestOfRunInd,StdFitnessWorstOfRunInd,MeanTreeSizeOfRun,MeanTreeDepthOfRun,TreeSizeBestOfRunInd,TreeDepthBestOfRunInd,TreeSizeWorstOfRunInd,TreeDepthWorstOfRunInd

dat <- read.table("/home/ggerules/lilgp1.03/app/reports/sttlong.csv", header=TRUE, sep=",", na.strings="NA", dec=".", strip.white=TRUE)

#dat sorted
dats <- with(dat, dat[order(ProblemNum, Kernel, wADF, ADF, Types, Constraints, acgpwhat, IndRunNum, decreasing=FALSE), ])

##write.table(dats, file = "sttlong.csv", sep = ",", col.names = TRUE, row.names = TRUE, qmethod = "double")
#write.table(dats, file = "sttlongsorted.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

#dim(dats)
#str(dats)
#class(dat)
#class(dats)

##### kernel_orig
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      #filter(ProblemNum == 3, Kernel == 0, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "n")
      filter(ProblemNum == 3, Kernel == 0, wADF == "y", Types == "n", Constraints == "n", acgpwhat == "n")

#write.table(d, file = "3_gtsbgi2_orig_kern.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

#typeof(d)

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 0, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "n")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 0, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "n")

x <- c(o1$GenNum, o2$GenNum) 
str(x)
framework <- c(rep("orig_nadf",51), rep("orig_yadf",51))
str(framework)
val <- c(o1$mean, o2$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="Original kernel.orig Used\n 30 Independent Runs\n (wADF = y) ", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_orig_yadf.pdf")

##### kernel.cgp2.1
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 1, wADF == "n")

#write.table(d, file = "3_gtsbgi2_cgp2p1_nwadf.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 1, wADF == "n", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "n")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 1, wADF == "n", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "n")

x <- c(o1$GenNum, o2$GenNum) 
str(x)
framework <- c(
 rep("cgp2.1_ntypes_ncons",51), 
 rep("cgp2.1_ntypes_ycons",51), 
 rep("cgp2.1_ytypes_ncons",51), 
 rep("cgp2.1_ytypes_ycons",51)
)
str(framework)
val <- c(o1$mean, o2$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="Original  kernel.cgp2.1 Used\n 30 Independent Runs\n (wADF = n, ADF = n)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_cgp2p1_nwadf.pdf")

##### kernel.cgp2.1_adf
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(Kernel == 2)

#write.table(d, file = "3_gtsbgi2_cgp2p1_ywadf.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 2, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "n")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 2, wADF == "y", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "n")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 2, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "n")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 2, wADF == "y", ADF == "y", Types == "y", Constraints == "n", acgpwhat == "n")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 
str(x)
framework <- c(
 rep("cgp2.1_nadf_ntypes",51), 
 rep("cgp2.1_nadf_ytypes",51), 
 rep("cgp2.1_yadf_ntypes",51), 
 rep("cgp2.1_yadf_ytypes",51)
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="New kernel.cgp2.1_adf Used\n 30 Independent Runs\n (wADF = y)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_cgp2p1_ywadf.pdf")

##### kernel.acgp1p1p2 no constraints 
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(Kernel == 3)

#write.table(d, file = "3_gtsbgi2_acgp1p1p2.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 3, wADF == "n", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "0")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 3, wADF == "n", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "1")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 3, wADF == "n", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "2")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 3, wADF == "n", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "3")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 

str(x)
framework <- c(
 rep("acgp1.1.2_what0",51), 
 rep("acgp1.1.2_what1",51), 
 rep("acgp1.1.2_what2",51), 
 rep("acgp1.1.2_what3",51) 
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="Original kernel.acgp1.1.2  Used\n 30 Independent Runs\n (wADF = n, ADF = n, Types = n)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_acgp1p1p2.pdf")

##### kernel.acgp2.1 what0
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, acgpwhat == "0")

#write.table(d, file = "3_gtsbgi2_acgp2p1_ywadf_what0.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "0")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "0")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "0")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "y", Constraints == "n", acgpwhat == "0")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 
str(x)
framework <- c(
 rep("acgp2.1_nafd_ntypes",51), 
 rep("acgp2.1_nafd_ytypes",51), 
 rep("acgp2.1_yafd_ntypes",51), 
 rep("acgp2.1_yafd_ytypes",51) 
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="New kernel.acgp2.1 Used\n30 Independent Runs\n (wADF = y, what = 0)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_acgp2.1_what0.pdf")

##### kernel.acgp2.1 what1
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, acgpwhat == "1")

#write.table(d, file = "3_gtsbgi2_acgp2p1_ywadf_what1.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "1")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "1")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "1")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "y", Constraints == "n", acgpwhat == "1")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 
str(x)
framework <- c(
 rep("acgp2.1_nafd_ntypes",51), 
 rep("acgp2.1_nafd_ytypes",51), 
 rep("acgp2.1_yafd_ntypes",51), 
 rep("acgp2.1_yafd_ytypes",51) 
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="New kernel.acgp2.1 Used\n30 Independent Runs\n (wADF = y, what = 1)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_acgp2.1_what1.pdf")

##### kernel.acgp2.1 what2
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, acgpwhat == "2")

#write.table(d, file = "3_gtsbgi2_acgp2p1_ywadf_what2.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "2")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "2")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "2")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "y", Constraints == "n", acgpwhat == "2")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 
str(x)
framework <- c(
 rep("acgp2.1_nafd_ntypes",51), 
 rep("acgp2.1_nafd_ytypes",51), 
 rep("acgp2.1_yafd_ntypes",51), 
 rep("acgp2.1_yafd_ytypes",51) 
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="New kernel.acgp2.1 Used\n30 Independent Runs\n (wADF = y, what = 2)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_acgp2.1_what2.pdf")

##### kernel.acgp2.1 what3
d <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, acgpwhat == "3")

#write.table(d, file = "3_gtsbgi2_acgp2p1_ywadf_what3.csv", sep = ",", col.names = TRUE, row.names = FALSE, qmethod = "double")

o1 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "n", Constraints == "n", acgpwhat == "3")

o2 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "n", Types == "y", Constraints == "n", acgpwhat == "3")

o3 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "n", Constraints == "n", acgpwhat == "3")

o4 <- dats %>% 
      group_by(ProblemNum,Kernel,wADF,ADF,Types,Constraints,acgpwhat,GenNum) %>% 
      #summarise(num = n()) %>%
      summarise(mean = mean(StdFitnessBestOfGenInd)) %>%
      filter(ProblemNum == 3, Kernel == 4, wADF == "y", ADF == "y", Types == "y", Constraints == "n", acgpwhat == "3")

x <- c(o1$GenNum, o2$GenNum ,o3$GenNum, o4$GenNum) 
str(x)
framework <- c(
 rep("acgp2.1_nafd_ntypes",51), 
 rep("acgp2.1_nafd_ytypes",51), 
 rep("acgp2.1_yafd_ntypes",51), 
 rep("acgp2.1_yafd_ytypes",51) 
)
str(framework)
val <- c(o1$mean, o2$mean, o3$mean, o4$mean)
str(val)

df <- data.frame(x, framework, val)

plot <- ggplot(df, aes(x=x, y=val, group=framework, color=framework, linetype = framework)) + geom_line() +
  labs(title="Two Box - Mean of Tree Size of Best of Gen Individuals\n by Generation Number", subtitle="New kernel.acgp2.1 Used\n30 Independent Runs\n (wADF = y, what = 3)", x="Generation Number", y="Mean Tree Size Best of Gen Individuals")  + ylim(0,100)
ggsave(plot, file="3_gtsbgi2_acgp2.1_what3.pdf")


