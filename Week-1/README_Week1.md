# Multithread Mayhem - Week 1 Submission

## Name

Ayushman Soni

## Roll Number

24B0679

## Problems Attempted

- Problem 1 – Run, Observe, Write Down
- Problem 2 – Make the Race Worse

---

# Problem 1

## hello_thread

Maine program ko multiple times run kiya. Har run me saare 5 threads execute hue aur end me main thread ne "all done" print kiya.

Interesting baat ye thi ki thread execution ka order fixed nahi tha. Theory padhne se pehle mujhe lagta tha ki output hamesha same order me aayega, lekin run karne ke baad samajh aaya ki scheduler decide karta hai kaunsa thread kab chalega.

### Observation

- Saare threads successfully execute hue.
- Output ka order guaranteed nahi hai.
- `pthread_join()` ensure karta hai ki main thread jaldi exit na kare.

---

## race_counter

Expected output 20000000 tha, lekin mujhe kisi bhi run me correct answer nahi mila.

### Outputs observed

- 12471651
- 11971886
- 11468912
- 13284898
- 17272721

Pehle mujhe laga code me koi bug hoga, lekin theory padhne ke baad samajh aaya ki problem race condition ki wajah se aa rahi hai. Dono threads same counter ko ek saath modify kar rahe the aur kuch increments lose ho rahe the.

### Observation

- Har run me alag answer mila.
- Koi bhi run expected value tak nahi pahucha.
- Shared variable ko bina synchronization ke modify karna unsafe hai.

---

## parallel_sum

Expected sum 10000000 tha.

Observed outputs:

- 6557908
- 5188380
- 5353841

Ye example dekhkar samajh aaya ki race condition sirf counters tak limited nahi hoti. Real computations me bhi same issue aa sakta hai.

### Observation

- Result har run me alag tha.
- Shared variable `total` par concurrent updates ho rahe the.
- Kaafi additions lose ho gayi thi.

---

## bank_chaos

Ye example sabse interesting laga kyunki ye real-world banking logic jaisa lag raha tha.

Output me Alice ne saare successful withdrawals kar liye aur Bob mostly reject ho gaya. Final balance 0 tha, lekin code ko dekhkar samajh aaya ki check aur withdrawal alag-alag operations hain. Agar timing thodi different hoti to inconsistent state aa sakti thi.

### Observation

- Check-then-act pattern dangerous ho sakta hai.
- Banking, inventory aur ticket booking systems me aise bugs aa sakte hain.
- Shared state ko protect karna zaroori hai.

---

# Problem 2 – Make the Race Worse

Original code:

```c
counter++;
```

Maine ise modify karke:

```c
long temp = counter;
usleep(1);
temp = temp + 1;
counter = temp;
```

kar diya.

### Reason

Race condition read aur write ke beech hoti hai. Maine `usleep(1)` add karke us gap ko bada kar diya taaki dusra thread bhi same value read kar sake.

Is modification ke baad race condition aur clearly visible ho gayi aur incorrect outputs aur frequently aane lage.

---

# What Surprised Me

Mujhe sabse zyada surprise is baat se hua ki same code aur same input hone ke baad bhi output har run me different aa raha tha. Pehli baar practically dekha ki concurrent programs deterministic nahi hote.

---

# What I Learned

- Program, Process aur Thread me difference
- Threads kaunsa memory share karte hain
- Race condition kaise hoti hai
- `pthread_create()` aur `pthread_join()` ka use
- Shared variables ko synchronize karna kyu zaroori hai
- Concurrent programs ko debug karna normal programs se zyada difficult hota hai
