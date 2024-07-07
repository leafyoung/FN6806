## github - replit

rm main main.o main-debug
mkdir 71-multithread_mc_pi
mv *.h *.cpp 71-multithread_mc_pi/
git init
git remote add origin https://github.com/leafyoung/FN6806.git
git sparse-checkout init --cone
git sparse-checkout set 71-multithread_mc_pi
git fetch
git pull origin main

git add 71-multithread_mc_pi
git commit -m "71-multithread_mc_pi"
git rebase origin/main
git push -u origin main
