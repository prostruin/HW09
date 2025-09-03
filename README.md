# HW09
HW

## - 턴 로직

  - GameModeBase에 턴을 만들기위한 함수와 변수를 생성
    
    <img width="414" height="269" alt="image" src="https://github.com/user-attachments/assets/d4b5c9ab-b462-4d08-85fa-c1d665da80f9" />

처음에 현재 턴을 진행하고있는 플레이어를 알수있도록 CurrentPlayerIndex를 구현 BeginPlay에서 -1로 초기화 후

 - BeginTurn 에서 턴을 각각 인원수비례하여 한명씩 돌아가도록하고 타이머 시작.
 - 턴이 바뀔때마다 UI에서 보여지는 텍스트가 달라짐.
<img width="1094" height="729" alt="image" src="https://github.com/user-attachments/assets/ab77bb6a-4758-4479-8e60-d5fc5a86f7a6" />

- 타이머가 종료되거나 미리 정답을 입력하면 넘어감.
  <img width="1016" height="644" alt="image" src="https://github.com/user-attachments/assets/21244c85-489e-462b-9511-ad63d4b6d58f" />

- 각각의 턴이 종료되었을시
  <img width="718" height="341" alt="image" src="https://github.com/user-attachments/assets/631042f0-f01c-4417-afa9-f2b4f3283bd0" />

- PrintChatMessageString <- 입력 관련로직에서 만약 숫자 3개를 입력한다면. OnTurnFinished()가 실행되어서 다음턴으로 넘어감.

- 게임모드의 OnPostLogin에서 2명 이상이 입장하고나서 게임이 시작됨 - 2인게임용
  <img width="750" height="230" alt="image" src="https://github.com/user-attachments/assets/a2ac66e7-662e-47bc-8253-a5ece5f5dac4" />


- PlayerController에 턴 관련된 UI Text와 기능
<img width="196" height="54" alt="image" src="https://github.com/user-attachments/assets/9851bf10-4477-4b62-8f46-4f21c49f7022" />
<img width="529" height="125" alt="image" src="https://github.com/user-attachments/assets/bbe1eed6-39c7-43b5-a3b8-751e10d3fcaa" />

- 생성자에서 초기화해주고 각각 타이머 텍스트와 턴을 관리하는 값을 리플리케이트 해줌
<img width="901" height="195" alt="image" src="https://github.com/user-attachments/assets/d813fee0-2056-4a68-95d7-8d1de55ba685" />



