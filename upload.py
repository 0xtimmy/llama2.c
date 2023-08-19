from transformers import AutoModel

model = AutoModel.from_pretrained("./out/ckpt.py")
model.push_to_hub("0xtimmy/tinyllamas-exp2")