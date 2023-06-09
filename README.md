# Aho-Corasick Algorithm

 <h3>Aho-Corasick Algorithm </h3>
 
 <p>
多模式字串配對算法
<hr>
搆建一個Trie結合KMP方法給每個節點加上失配(fail)指針。
<br>每次失配就會用fail指針往回跳到其他模式串包括自己的前綴中匹配當前狀態的最長後綴的最尾字符。
<br><br>另外fail指針在查詢時也會用來獲取所有配對後綴,並且在某分支配對結束時跳回其他模式串或者自己的前綴的重複前綴最尾字符繼續配對。
<br>例子1:
<br>設3個模式字串"she","he"和"e", 當字串配對到分支"she"的最後字符"e"時,代表"he"和"e"這2個後綴也應該要配對到, 這時要利用fail指針來獲取另外2個分支的結果。
<br>例子2:
<br>設2個模式字串"hersh","she", 當字串配對到分支"hersh"最後結束時,利用fail指針直接跳向分支"she"的第二字符"h"繼續配對。

<hr>
不過失配時用fail指針跳到新節點之後還是要再配對一次子節點, 這裡配對指針會產生停頓, 而這時可以針對這部分改善
<br><br>其實可以觀察到fail指針指向的節點的字符一定跟自己一樣
<br>, 這時可以把fail指針指向的節點的子節點map中所有子節點納入自己的子節點map,但要優先保留自己孩子,忽略與自己孩子衝突的節點,只納入沒衝突的。child map -> transfer map
<br>, 這樣字典樹就變成字典圖, 在配對過程中就算失配或者配對結束時都可以跟據主字串的下一個字符像自己子節點一樣直接跳到合適的分支的子節點或者根繼續配對,配對指針沒有任何停頓。
<br>, fail指針只拿來獲取後綴。
<br><br>需要留意的是這項優化在搆建transfer map的時候會要求fail指針指向的上層節點已搆建好其fail指針和transfer map, 然後再用該transfer map搆建自己的transfer map
<br>, 所以搆建時要用廣度優先搜索


<hr>
<div>References:</div>
<ul>
<li>1. https://oi-wiki.org/string/ac-automaton/</li>
</ul>
</p>
